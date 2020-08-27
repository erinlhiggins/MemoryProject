/**
@file CorrectionScheme.h
@brief Abstract class parent for all correction schemes
@author Donald Kline, Jr
*/
#ifndef PRES_SIM_CORRECTION_SCHEME_H
#define PRES_SIM_CORRECTION_SCHEME_H
#include "PresSubConfig.h"
#include "FaultMap.h"
#include "GenUtils.h"
#include "OverheadPacket.h"
#include <sstream>
#include <vector>
#include <algorithm>

namespace PresSim {

/**
@class CorrectionScheme
@brief Abstract parent class for CorrectionSchemes
*/
class CorrectionScheme{

	public:
		CorrectionScheme(const PresSubConfig& psc) {
			//Step1: psc name is the parent_id
			parent_id = psc.get_name();
			//Step2: extract the required fault map models. 
			string all_models;

			if (psc.get_value(all_models, "fault_representations"))
    		{
				//These models will be used when link_maps() is called. 	
				stringstream ss(all_models);
				string str;
				while (getline(ss, str, ',')){
					model_strings.push_back(str);
				}
    		}

		};
		virtual void encode_data (uint64_t addr, PresSim::OverheadPacket&, const std::vector<uint8_t>& old_data, 
			const std::vector<uint32_t>& observed_faultybits, PresStatistics&) = 0;
		virtual uint32_t encode_dataless(uint64_t addr, uint32_t num_bits, const std::vector<uint32_t>& observed_faultybits, 
			PresStatistics&) = 0;
		virtual void decode_data (uint64_t addr, PresSim::OverheadPacket&, PresStatistics&) = 0;
		virtual uint32_t decode_dataless (uint64_t addr, uint32_t num_bits, PresStatistics&) = 0; 
		virtual uint32_t encoding_overhead (uint32_t data_size_in) = 0; 	
		virtual uint32_t decoding_overhead (uint32_t data_and_aux_in) = 0;
		virtual void registerStatNames(bool (*stat_register)(const string s)) = 0;
		virtual string getRegistrationID() final {return parent_id;};
		virtual void tick() = 0;

		//Non-Overrideable function
		virtual bool link_map(const string id, FaultRepresentation* fr) final{
			if (std::find(model_strings.begin(), model_strings.end(), id) != model_strings.end())
			{
				//Add to queryList;
				queryList.push_back(fr);
				//TODO sort and place based on model_strings order
				return true;
			}
			return false;
		};

	protected:
		virtual bool queryFaultFromChild(int query_index, uint64_t address, OverheadPacket& op, uint32_t bitposition) final{
			return queryList[query_index]->isVulnerable(address, op, bitposition);
		};

		virtual std::vector<uint32_t>* queryFaultsFromChild(int query_index, uint64_t address, OverheadPacket& op, uint32_t start, uint32_t end) final {
			return queryList[query_index]->isVulnerableRange(address, op, start, end);
		};

		virtual std::vector<uint32_t> getAllFaults(uint64_t address, OverheadPacket& op, uint32_t size){
			std::vector<uint32_t> locations;
			for (int i=0; i<queryList.size(); i++)
			{
		        std::vector<uint32_t>* new_locations = queryFaultsFromChild(i, address, op, 0, size);
        		locations.insert(locations.end(), new_locations->begin(), new_locations->end());
			}
			return locations;
		}

		virtual std::vector<uint32_t> getAllWeaknesses(uint64_t address, uint32_t size){
			std::vector<uint32_t> locations;
			for (int i=0; i<queryList.size(); i++)
			{
		        std::vector<uint32_t>* new_locations = queryWeaknessesFromChild(i, address, 0, size);
        		locations.insert(locations.end(), new_locations->begin(), new_locations->end());
			}
			return locations;
		}



		virtual bool queryWeaknessFromChild(int query_index, uint64_t address, uint32_t bitposition) final{
			return queryList[query_index]->isWeak(address, bitposition);
		};

		virtual std::vector<uint32_t>* queryWeaknessesFromChild(int query_index, uint64_t address, uint32_t start, uint32_t end) final{
			return queryList[query_index]->isWeakRange(address, start, end);
		};

		
		string parent_id;
		std::vector<string> model_strings;
	private:
		std::vector<FaultRepresentation*> queryList;

};
} //end namespace
#endif
