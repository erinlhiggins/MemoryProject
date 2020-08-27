/**
@file PresSubConfig.h
@brief Class definition for PresSubConfig
@author Donald Kline Jr, University of Pittsburgh
*/
#ifndef PRES_SIM_UTIL_PRES_CORRECTION_CONFIG_BASE_H
#define PRES_SIM_UTIL_PRES_CORRECTION_CONFIG_BASE_H

using namespace std;
#include "GenUtils.h"

namespace PresSim {
/**
@class PresSubConfig
@brief Individual configuration options for correction schemes or fault models
*/
class PresSubConfig{
	
	public:
		PresSubConfig(string name);
		PresSubConfig(PresSubConfig & psc);
		PresSubConfig(PresSubConfig && psc);
		PresSubConfig & operator = (PresSubConfig &);
		PresSubConfig & operator = (PresSubConfig &&);

		bool parse_line(const string& line);
		/**
		@fn get_name() const
		@brief Retrieve the name of the module this SubConfig is tied to
		*/
		string get_name() const {return my_name;};
		bool get_value(string& value, const string& name) const;
		bool get_value(int& value, const string& name) const;
		bool get_value(uint32_t& value, const string& name) const;
		bool get_value(long& value, const string& name) const;
		bool get_value(uint64_t& value, const string& name) const;
		bool get_value(bool& value, const string& name) const;
		bool get_value(double& value, const string& name) const;
		bool get_value(float& value, const string& name) const;
	private:
		string my_name;
		std::map<string, string> param_map;
};
} //end namespace PresSim

#endif
