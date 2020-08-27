/**
@file PresSim.cc
@brief Implementation of ReliabilitySimulator class from PresSim.h
@author Donald Kline, Jr University of Pittsburgh
*/

#include "PresSim.h"
#include "src/correction_schemes/AUTOGEN_CS.h"
#include "src/fault_models/AUTOGEN_FM.h"
#include "src/fault_maps/AUTOGEN_FMAP.h"

using namespace PresSim;

/**
@fn ReliabilitySimulator::ReliabilitySimulator(string config_filename)
@brief Initializes a Reliability Simulator
@param config_filename string with configuration values
@warning calls abort() on invalid configuration
*/
ReliabilitySimulator::ReliabilitySimulator(string config_filename) : 
	m_ticks(0),
	m_tick_rate_correction(1),
	m_tick_rate_model(1),
	m_data_size(512),
	m_maximum_overhead(0),
	m_output_file(NULL_STR_PRES),
	m_global_statistics()
 	{
    
	//Configuration throws abort if invalid, so we know here configuration is valid
	PresConfig my_base(config_filename); //not named m_base because it is local, not a class variable	

//Take all non-correction scheme and non-faultmodel parameters here for setup
	long x = 0;
	if (my_base.myConfig.get_value(x, "tick_start"))
		m_ticks=x;
	if (my_base.myConfig.get_value(x, "tick_rate"))
	{
        m_tick_rate_correction=x;
		m_tick_rate_model=x;
	}
	if (my_base.myConfig.get_value(x, "tick_rate_correction"))
        m_tick_rate_correction=x;
	if (my_base.myConfig.get_value(x, "tick_rate_model"))
        m_tick_rate_model=x;

	int y = 0;

	if (my_base.myConfig.get_value(y, "data_size"))
		m_data_size=y;

	string s = NULL_STR_PRES;
	if (my_base.myConfig.get_value(s, "output_file"))
		m_output_file = s;
//end of parameter setup

//Statistics setup
	m_global_statistics.addRootStat("write_bit_flips");
	m_global_statistics.addRootStat("read_bit_flips");
	m_global_statistics.addRootStat("write_fails");
	m_global_statistics.addRootStat("read_fails");
	m_global_statistics.addRootStat("total_bits_written");
	m_global_statistics.addRootStat("total_bits_read");
	m_global_statistics.addRootStat("total_clock_ticks");

//End Statistics setup

	m_FaultMap = std::move(PRES_FACTORY_create_fault_map(my_base.myFaultMap));

	//Create a vector of fault models, built from the fault model sub configurations

	for (auto it = my_base.fault_models.begin(); it!=my_base.fault_models.end(); ++it)
	{

		std::unique_ptr<FaultModel> tmp = std::move(PRES_FACTORY_create_fault_model(*it));
		m_models.push_back(std::move(tmp));		
	}	

	//Create a vector of correction models, built from the correction model sub configurations
	//Note that this also builds the maximum overhead
	for (auto it = my_base.correction_schemes.begin(); it!=my_base.correction_schemes.end(); ++it)
	{

		std::unique_ptr<CorrectionScheme> tmp = std::move(PRES_FACTORY_create_correction_scheme(*it));
		m_maximum_overhead+=tmp->encoding_overhead(m_data_size+m_maximum_overhead);
		for (auto it2 = m_models.begin(); it2!=m_models.end(); it2++)
		{
			//TODO change fault model link to shared pointer, kind of cheating unique pointer
			tmp->link_map((*it2)->get_id(), static_cast<FaultRepresentation*>((*it2).get()));

		}
		m_corrections.push_back(std::move(tmp));		
	}	

	//If here, successfully initialized PresSim with no errors
}


/**
@fn ReliabilitySimulator::tick()
@brief Calls tick (every tick rate) for each fault model and correction scheme
*/
void ReliabilitySimulator::tick(){
	//call .tick on all fault models	
	m_ticks++;
	if (m_tick_rate_model>0 && m_ticks%m_tick_rate_model==0)
	{	
		for (auto it = m_models.begin(); it!=m_models.end(); ++it)
			(*it)->tick();
	}
	if (m_tick_rate_correction>0 && m_ticks%m_tick_rate_correction==0)
	{

		for (auto it = m_corrections.begin(); it!=m_corrections.end(); ++it)
			(*it)->tick();
	} 	

	//TODO update myFaultMap fault entries on tick()
}

ReliabilitySimulator::~ReliabilitySimulator(){

	m_global_statistics.updateRootStat("total_clock_ticks", m_ticks);
	
	if (m_output_file == NULL_STR_PRES)
		m_global_statistics.printStats(std::cout);	
	else{
		std::ofstream ofs (m_output_file, std::ofstream::out);
		m_global_statistics.printStats(ofs);
		ofs.close();
	}
}

void ReliabilitySimulator::update_stats(const OverheadPacket& op_orig, const OverheadPacket& op_new, bool writing){
	std::tuple<uint32_t, uint32_t> faults = bitFlipCount(&op_orig.data_bits[0], &op_new.data_bits[0], op_orig.data_bits.size());
	uint32_t failures = get<0>(faults) + get<1>(faults);	
	if (writing){
		if (failures>0){
			cout<<"Saw "<<failures<<" failures during write"<<endl;
			op_orig.print_data();
			op_new.print_data(); 	
			m_global_statistics.updateRootStat("write_bit_flips", failures);
    		m_global_statistics.updateRootStat("write_fails", 1);
		}
		m_global_statistics.updateRootStat("total_bits_written", 8*op_orig.data_bits.size()+op_orig.aux_bits.size());
	}else{
		if (failures>0){
			cout<<"Saw "<<failures<<" failures during read"<<endl;
			op_orig.print_data();
			op_new.print_data(); 	

			m_global_statistics.updateRootStat("read_bit_flips", failures);
    		m_global_statistics.updateRootStat("read_fails", 1);
		}
    	m_global_statistics.updateRootStat("total_bits_read", 8*op_orig.data_bits.size()+op_orig.aux_bits.size());	
	}
}

/**
@fn ReliabilitySimulator::perform_write(uint64_t address, std::vector<uint8_t>& data_old, std::vector<uint8_t>& data_new)
@param address Address being written to
@param data_old may be empty. If not same size as data_new, will be treated as empty
@param data_new the data you are planning to write
@return time in picoseconds to perform the corrections. UINT64_MAX indicates error during the writing
@brief Calculate bitflips and correction for a read of this data at this address
*/
uint64_t ReliabilitySimulator::perform_write(uint64_t address, const std::vector<uint8_t>& data_old, const std::vector<uint8_t>& data_new){
	//Verify legal data
	if (data_new.size()==0) return UINT64_MAX; 

	//1) Create overhead packet
	OverheadPacket orig(data_new.size(), m_maximum_overhead);
	orig.data_bits = data_new; //copy

	OverheadPacket adapt(data_new.size(), m_maximum_overhead);
	adapt.data_bits = data_new; //copy


	//2) Create 

	for (auto it = m_corrections.begin(); it!= m_corrections.end(); ++it)
	{
		m_global_statistics.setCachedLabel((*it)->getRegistrationID());	
		
		(*it)-> encode_data (address, adapt, data_old,  m_FaultMap.get()->get_faults(address),
           	m_global_statistics);
		adapt.addbaseaux((*it)->encoding_overhead(adapt.current_overhead()));
	}

	for (auto it = m_models.begin(); it!=m_models.end(); ++it)
	{
		
		(*it)->apply_write(address, adapt);

	}
	//Data in "adapt" at this stage is what is actually stored

	//Note: The decode is to demonstrate that the reliability scheme will be able to recover the current errors
	for (auto rit = m_corrections.rbegin(); rit!= m_corrections.rend(); ++rit)
	{
		m_global_statistics.setCachedLabel((*rit)->getRegistrationID());
		adapt.subtractbaseaux((*rit)->decoding_overhead(adapt.current_overhead()));		
        (*rit)-> decode_data(address, adapt, m_global_statistics);
	}
	//3) Track errors
	update_stats(orig, adapt, true);
	//NOTE: these errors won't be actually realized until the read unless read-after-write is used
	
	//4 Return the delay
	return adapt.get_stats()[0];
}

/**
@fn ReliabilitySimulator::perform_read(uint64_t address, std::vector<uint8_t>& data)
@brief Calculate bitflips and correction for a read of this data at this address
@param address the address to read from
@param golden_data The data that you are reading from address. Fault free, just the expected data to be read. 
@return time in ps it took to perform the corrections. UINT64_MAX on error
*/
uint64_t ReliabilitySimulator::perform_read(uint64_t address, const std::vector<uint8_t>& golden_data){
	//Verify legal data
    if (golden_data.size()==0) return UINT64_MAX;

	//1) Create overhead packet
    OverheadPacket orig(golden_data.size(), m_maximum_overhead);
    orig.data_bits = golden_data; //copy

    OverheadPacket adapt(golden_data.size(), m_maximum_overhead);
    adapt.data_bits = golden_data; //copy

    //2) Create 

	//This data encoding is to prevent having to store the encoded values. 
	for (auto it = m_corrections.begin(); it!= m_corrections.end(); ++it)
    {
        m_global_statistics.setCachedLabel((*it)->getRegistrationID());

        (*it)-> encode_data (address, adapt, golden_data, m_FaultMap.get()->get_faults(address),
            m_global_statistics);
		adapt.addbaseaux((*it)->encoding_overhead(adapt.current_overhead()));
    }

    for (auto it = m_models.begin(); it!=m_models.end(); ++it)
	{
        (*it)->apply_read(address, adapt);
    }

	for (auto rit = m_corrections.rbegin(); rit!= m_corrections.rend(); ++rit)
    {
        m_global_statistics.setCachedLabel((*rit)->getRegistrationID());
		adapt.subtractbaseaux((*rit)->decoding_overhead(adapt.current_overhead()));
        (*rit)-> decode_data(address, adapt, m_global_statistics);
    }

    //3) Track errors 
    update_stats(orig, adapt, false);
	
    //4 Return
    return adapt.get_stats()[0]; 

}

// Basically just an entry in the symbol table
extern "C"
{
	void libpressim_is_present(void)
	{
		;
	}
}
