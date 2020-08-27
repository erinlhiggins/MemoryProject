/**
@file StaticFaultModel.h
@brief File for abstract class definition of StaticFaultModel
@author Donald Kline, Jr
*/

#ifndef PRES_SIM_STATIC_FAULT_MODEL_H
#define PRES_SIM_STATIC_FAULT_MODEL_H

#include "GenUtils.h"
#include "OverheadPacket.h"
#include "PresSubConfig.h"
#include "FaultModel.h"
#include <cstring>

namespace PresSim{
/**
@class StaticFaultModel
@brief Abstract parent class for fault models which use a static map
*/
class StaticFaultModel : public FaultModel{
    public:
        StaticFaultModel(const PresSubConfig& ps);
		~StaticFaultModel();
        virtual bool isVulnerable(uint64_t address, OverheadPacket& data, uint32_t position) = 0;
        virtual bool is_deterministic() final{return true;};

        virtual void apply_read(uint64_t addr,OverheadPacket& op) = 0;
        virtual void apply_write(uint64_t addr,OverheadPacket& op) = 0;
        virtual void tick() = 0;
		virtual bool is_mappable() final {return true;}; 

	protected:
		bool** m_faults;
        uint32_t m_rows;
        uint32_t m_cols;
	private:
		void read_faults(string faultmodel_name); 
		void get_faults_size(string faultmodel_name);
};

} //end namespace PresSim

#endif
