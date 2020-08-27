/**
@file FaultRepresentation.h
@brief File for abstract class definition of FaultRepresentation. Used by CorrectionScheme
*/

#ifndef PRES_SIM_FAULT_REPRESENTATION_H
#define PRES_SIM_FAULT_REPRESENTATION_H

#include "GenUtils.h"
#include "OverheadPacket.h"
#include "PresSubConfig.h"

namespace PresSim{
/**
@class FaultModel
@brief Abstract parent class for FaultModel and FaultMap
*/
class FaultRepresentation{
	public:
		virtual bool isVulnerable(uint64_t address, OverheadPacket& data, uint32_t position) = 0;
		virtual std::vector<uint32_t>* isVulnerableRange(uint64_t address, OverheadPacket& data, uint32_t start, uint32_t end) = 0;
        virtual bool isWeak(uint64_t address, uint32_t position) = 0;
        virtual std::vector<uint32_t>* isWeakRange(uint64_t address,  uint32_t start, uint32_t end) = 0;
		virtual bool is_mappable() = 0;

};
}
#endif
