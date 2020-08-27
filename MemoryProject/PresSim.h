/**
@file
@brief Base include for PresSim simulator
@author Donald Kline, Jr
*/
#ifndef PRES_SIM_RELIABILITY_SIMULATOR
#define PRES_SIM_RELIABILITY_SIMULATOR

#include "src/utils/pres_utils.h"

using namespace std;
namespace PresSim{

/**
@class ReliabilitySimulator
@brief Integrates modular fault models and correction schemes to calculate memory fault rates

*/
class ReliabilitySimulator{
    public:
    ReliabilitySimulator(std::string config_filename);
	~ReliabilitySimulator();
	void tick();
	uint64_t perform_read(uint64_t address, const std::vector<uint8_t>& data);  
	uint64_t perform_write(uint64_t address, const std::vector<uint8_t>& data_old, const std::vector<uint8_t>& data_new);  
    private:
	uint64_t m_ticks;
	uint64_t m_tick_rate_correction;
	uint64_t m_tick_rate_model;
	uint32_t m_data_size;
	uint32_t m_maximum_overhead;
	unique_ptr<FaultMap> m_FaultMap;
	vector<unique_ptr<FaultModel>> m_models;
	vector<unique_ptr<CorrectionScheme>> m_corrections;
	string m_output_file;
	PresStatistics m_global_statistics;
	private:
		void update_stats(const OverheadPacket& op_orig, const OverheadPacket& op_new, bool writing);
};

}
#endif
