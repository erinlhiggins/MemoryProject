/**
@file
@brief Empty Fault Map (all no-ops)
@author Donald Kline, Jr
*/
#ifndef PRES_SIM_EMPTY_FAULT_MAP_H
#define PRES_SIM_EMPTY_FAULT_MAP_H

#include "../utils/FaultMap.h"
#include "../utils/PresSubConfig.h"

/**
@class EmptyFMap : public PresSim::FaultMap
@brief 3 faults in every row, at the same position (toy example)
*/
class EmptyFMap : public PresSim::FaultMap{

	public:
		//Combined Fault Map?
		EmptyFMap(const PresSim::PresSubConfig& psc) : FaultMap(psc){};
		bool isVulnerable(uint64_t address, PresSim::OverheadPacket& data, uint32_t position) {return false;};
		std::vector<uint32_t>* isVulnerableRange(uint64_t address, PresSim::OverheadPacket& data, uint32_t start, uint32_t end) {std::vector<uint32_t> tmp; return &tmp;};
        bool isWeak(uint64_t address, uint32_t position) {return false;};
        std::vector<uint32_t>* isWeakRange(uint64_t address,  uint32_t start, uint32_t end) {std::vector<uint32_t> tmp; return &tmp;};
		void add_fault(uint64_t address, uint32_t bit_offset, int id) {};
		std::vector<uint32_t> get_faults(uint64_t address) const {std::vector<uint32_t> tmp; return tmp;};
};


#endif
