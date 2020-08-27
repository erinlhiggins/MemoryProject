/**
@file
@brief FaultMap declarations
@author Donald Kline, Jr
*/

#ifndef PRES_SIM_FAULT_MAP_H
#define PRES_SIM_FAULT_MAP_H

#include "GenUtils.h"
#include "PresSubConfig.h"
#include "FaultRepresentation.h"

namespace PresSim{

/**
@class FaultMap
@brief Used by certain correction schemes to represent faults
*/
class FaultMap : public FaultRepresentation{

	public:
		//Combined Fault Map?
		//permanent faults, weakness
		FaultMap(const PresSim::PresSubConfig& psc) {};//: FaultRepresentation() {};
		virtual bool isVulnerable(uint64_t address, OverheadPacket& data, uint32_t position) = 0;
		virtual std::vector<uint32_t>* isVulnerableRange(uint64_t address, OverheadPacket& data, uint32_t start, uint32_t end) = 0;
        virtual bool isWeak(uint64_t address, uint32_t position) = 0;
        virtual std::vector<uint32_t>* isWeakRange(uint64_t address,  uint32_t start, uint32_t end) = 0;
		virtual void add_fault(uint64_t address, uint32_t bit_offset, int id) = 0; 
		virtual std::vector<uint32_t> get_faults(uint64_t address) const = 0;
		virtual bool is_mappable() final {return true;}
};

} //end namespace PresSim

#endif
