/**
@file FaultModel.h
@brief File for abstract class definition of FaultModel
*/

#ifndef PRES_SIM_FAULT_MODEL_H
#define PRES_SIM_FAULT_MODEL_H

#include "GenUtils.h"
#include "OverheadPacket.h"
#include "PresSubConfig.h"
#include "FaultRepresentation.h"

namespace PresSim{
/**
@class FaultModel
@brief Abstract parent class for all other fault models
*/
class FaultModel : public FaultRepresentation{

	public:
		FaultModel(const PresSubConfig& ps) {
			parent_id = ps.get_name();
		};//: FaultRepresentation() {};
		virtual bool isVulnerable(uint64_t address, OverheadPacket& data, uint32_t position) = 0;
        virtual std::vector<uint32_t>* isVulnerableRange(uint64_t address, OverheadPacket& data, uint32_t start, uint32_t end) = 0;
        virtual bool isWeak(uint64_t address, uint32_t position) = 0;
        virtual std::vector<uint32_t>* isWeakRange(uint64_t address,  uint32_t start, uint32_t end) = 0;
		virtual bool is_deterministic() = 0;	
		virtual bool is_mappable() = 0;		
		virtual void apply_read(uint64_t addr,OverheadPacket& op) = 0;
		virtual void apply_write(uint64_t addr,OverheadPacket& op) = 0;
		virtual void tick() = 0;
		virtual string get_id() final {return parent_id;};

	private:
		string parent_id;
};

} //end namespace PresSim

#endif
