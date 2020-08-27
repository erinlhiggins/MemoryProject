/**
@file
@brief Declaration of 3f Fault Map (toy fault map for testing)
@author Donald Kline, Jr
*/
#ifndef PRES_SIM_BASIC_3F_FAULT_MAP_H
#define PRES_SIM_BASIC_3F_FAULT_MAP_H

#include "../utils/FaultMap.h"
#include "../utils/PresSubConfig.h"

/**
@class Basic3f_fmap : public PresSim::FaultMap
@brief 3 faults in every row, at the same position (toy example)
*/
class Basic3f_fmap : public PresSim::FaultMap{

	public:
		//Combined Fault Map?
		//permanent faults, weakness
		Basic3f_fmap(const PresSim::PresSubConfig& psc);
		bool isVulnerable(uint64_t address, PresSim::OverheadPacket& data, uint32_t position);
        std::vector<uint32_t>* isVulnerableRange(uint64_t address, PresSim::OverheadPacket& data, uint32_t start, uint32_t end);
        bool isWeak(uint64_t address, uint32_t position);
        std::vector<uint32_t>* isWeakRange(uint64_t address,  uint32_t start, uint32_t end);
		void add_fault(uint64_t address, uint32_t bit_offset, int id);
		std::vector<uint32_t> get_faults(uint64_t address) const;

};


#endif
