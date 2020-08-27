/**
@file PresConfig.h
@brief Definitions for the class PresConfig, which handles the primary configuration parsing
*/
#ifndef PRES_SIM_UTIL_CONFIG_PARSER_BASE_H
#define PRES_SIM_UTIL_CONFIG_PARSER_BASE_H

#include "GenUtils.h"
#include "PresSubConfig.h"

namespace PresSim {
/**
@class PresConfig
@brief Configuration class, with global subconfig, and then vectors of correction/fault model sub configs
*/
class PresConfig{
	
	public:
		PresConfig(std::string filename);
		PresSim::PresSubConfig myConfig;
		PresSim::PresSubConfig myFaultMap;
		std::vector<PresSim::PresSubConfig> correction_schemes;
		std::vector<PresSim::PresSubConfig> fault_models;

};
}//end namespace

#endif
