/**
@file
@brief Class implementation for PresSubConfig
@author Donald Kline Jr, University of Pittsburgh
*/
#include <stdlib.h>
#include "PresSubConfig.h"

#define DELIM "="
using namespace PresSim;

static string trim(const string& s){
	int left=0, right=s.size()-1;
	for (; left<s.size();left++)
	{
		if (!isspace(s[left]))
			break;
	}
	for (;right>0; right--)
	{
		if (!isspace(s[right]))
			break;
	}
	if (left>right)
		return NULL_STR_PRES;
	return s.substr(left, right+1);
	

}


/**
@fn PresSubConfig::PresSubConfig(string name)
@brief constructs a sub config, which is used to initialize both CorrectionScheme and FaultModel objects
@param name registered class name for a correction scheme or fault model
@note This does no checking of the validaty of the class name. That is handled outside of this class. 
*/
PresSubConfig::PresSubConfig(string name) : my_name(trim(name)){}

/**
@fn PresSubConfig::PresSubConfig(PresSubConfig & psc)
@brief copy constructor
*/
PresSubConfig::PresSubConfig(PresSubConfig & psc)
{
	my_name = psc.my_name;
	param_map = psc.param_map;			
}

/**
@fn PresSubConfig::PresSubConfig(PresSubConfig&& psc)
@brief move constructor
*/
PresSubConfig::PresSubConfig(PresSubConfig&& psc)
{
	my_name = psc.my_name; //copy instead of move
	param_map = std::move (psc.param_map);
	psc.my_name = NULL_STR_PRES;
}

/**
@fn PresSubConfig::operator = (PresSubConfig& psc)
@brief copy operator
*/
PresSubConfig& PresSubConfig::operator = (PresSubConfig& psc)
{
	my_name = psc.my_name;
	param_map = psc.param_map;
	return *this;
}

/**
@fn PresSubConfig::operator = (PresSubConfig&& psc)
@brief move operator
*/
PresSubConfig& PresSubConfig::operator = (PresSubConfig && psc)
{
	
    my_name = psc.my_name; //copy instead of move
    param_map = std::move (psc.param_map);
    psc.my_name = NULL_STR_PRES;
	return *this;
}


/**
@fn PresSubConfig::parse_line(const string& line)
@brief Parses a config file line, and adds it to the parameter list if parsing is successful
@param line The line to parse
@return Whether or not an entry was added to the parameter list for this SubConfig
*/
bool PresSubConfig::parse_line(const string& line){
	std::size_t found = line.find(DELIM);
	if (found == std::string::npos)
		return false;

	string id = trim(line.substr(0,found));
	string val = trim(line.substr(found+1));
	if (id==NULL_STR_PRES || val==NULL_STR_PRES)
		return false;
	param_map[id]=val;

	return true;
}

/**
@fn PresSubConfig::get_value(string& value, const string& name) const
@brief Retrieve a requested parameter from the SubConfig configuration
@param value String reference to overwrite
@param name String name of parameter to retrieve
@return Whether or not 'name' was found
*/
bool PresSubConfig::get_value(string& value, const string& name) const{
	if (param_map.count(name)<=0)
		return false;
	value = param_map.at(name); 
	return true;
}

/**
@fn PresSubConfig::get_value(uint32_t& value, const string& name) const
@brief Retrieve a requested parameter from the SubConfig configuration
@param value uint32_t reference to overwrite
@param name String name of parameter to retrieve
@return Whether or not 'name' was found AND if the value stored with 'name' was correctly converted to uint32_t
*/
bool PresSubConfig::get_value(uint32_t& value, const string& name) const{
    if(param_map.count(name)<=0)
        return false; //name not found
    string s = param_map.at(name);
    if (!isdigit(s[0]) && s[0]!='-')
        return false; //value not valid as int
    for (int i=1; i<s.size(); i++)
        if (!isdigit(s[i]))
            return false;
    value = stoul(s);
    return true;
}

/**
@fn PresSubConfig::get_value(int& value, const string& name) const
@brief Retrieve a requested parameter from the SubConfig configuration
@param value Int reference to overwrite
@param name String name of parameter to retrieve
@return Whether or not 'name' was found AND if the value stored with 'name' was correctly converted to int
*/
bool PresSubConfig::get_value(int& value, const string& name) const{
	if(param_map.count(name)<=0)
        return false; //name not found
    string s = param_map.at(name);
	if (!isdigit(s[0]) && s[0]!='-')
		return false; //value not valid as int
	for (int i=1; i<s.size(); i++)
        if (!isdigit(s[i]))
            return false;
	value = stoi(s);
	return true;
}
		
/**
@fn PresSubConfig::get_value(long& value, const string& name) const
@brief Retrieve a requested parameter from the SubConfig configuration
@param value Long reference to overwrite
@param name String name of parameter to retrieve
@return Whether or not 'name' was found AND if the value stored with 'name' was correctly converted to long
*/
bool PresSubConfig::get_value(long& value, const string& name) const{
	if(param_map.count(name)<=0)
        return false; //name not found
    string s = param_map.at(name);
    if (!isdigit(s[0]) && s[0]!='-')
        return false; //value not valid as int
    for (int i=1; i<s.size(); i++)
        if (!isdigit(s[i]))
            return false;
    value = stol(s);
    return true;
}

/**
@fn PresSubConfig::get_value(uint64_t& value, const string& name) const
@brief Retrieve a requested parameter from the SubConfig configuration
@param value uint64_t reference to overwrite
@param name String name of parameter to retrieve
@return Whether or not 'name' was found AND if the value stored with 'name' was correctly converted to uint64_t
*/
bool PresSubConfig::get_value(uint64_t& value, const string& name) const{
    if(param_map.count(name)<=0)
        return false; //name not found
    string s = param_map.at(name);
    if (!isdigit(s[0]) && s[0]!='-')
        return false; //value not valid as int
    for (int i=1; i<s.size(); i++)
        if (!isdigit(s[i]))
            return false;
    value = stoull(s);
    return true;
}

/**
@fn PresSubConfig::get_value(double& value, const string& name) const
@brief Retrieve a requested parameter from the SubConfig configuration
@param value double reference to overwrite
@param name String name of parameter to retrieve
@return Whether or not 'name' was found AND if the value stored with 'name' was correctly converted to float
*/
bool PresSubConfig::get_value(double& value, const string& name) const
{
	if(param_map.count(name)<=0)
        return false; //name not found
    string s = param_map.at(name);
    if (!isdigit(s[0]) && s[0]!='-')
        return false; //value not valid as float (.25 not valid, need 0.25)

	bool saw_decimal = false;
    for (int i=1; i<s.size(); i++)
	{
        if (!isdigit(s[i]) && s[i]!='.')
            return false;
	}
	value = stod(s);
	return true;
}

/**
@fn PresSubConfig::get_value(float& value, const string& name) const
@brief Retrieve a requested parameter from the SubConfig configuration
@param value float reference to overwrite
@param name String name of parameter to retrieve
@return Whether or not 'name' was found AND if the value stored with 'name' was correctly converted to float
*/
bool PresSubConfig::get_value(float& value, const string& name) const
{
    if(param_map.count(name)<=0)
        return false; //name not found
    string s = param_map.at(name);
    if (!isdigit(s[0]) && s[0]!='-')
        return false; //value not valid as float (.25 not valid, need 0.25)

    bool saw_decimal = false;
    for (int i=1; i<s.size(); i++)
    {
        if (!isdigit(s[i]) && s[i]!='.')
            return false;
    }
    value = stof(s);
    return true;
}
		
/**
@fn PresSubConfig::get_value(bool& value, const string& name) const
@brief Retrieve a requested parameter from the SubConfig configuration
@param value bool reference to overwrite
@param name String name of parameter to retrieve
@note Accepted values for true (all strings ignore case):  start with 't', start with 'y', int/long value>0
@note Accepted values for false (all strings ignore case): start with 'f', start with 'n', int/long value=0
@return Whether or not 'name' was found AND if the value stored with 'name' was correctly converted to bool
*/
bool PresSubConfig::get_value(bool& value, const string& name) const{
	if(param_map.count(name)<=0)
		return false; //name not found
	string s = param_map.at(name);

	//look for true, false, yes, no strings
	if (tolower(s[0])=='t' || tolower(s[0])=='y'){
		value = true;
		return true;
	} else if (tolower(s[0])=='f' || tolower(s[0])=='n'){
		value = false;
		return true;
	}

	//See if this can be converted to long without error
	for (int i=0; i<s.size(); i++)
		if (!isdigit(s[i]))
			return false;
	
	long val = stol(s);
	value = val>0?true:false;
	return true;
}

