/**
@file
@brief Implementation of PresConfig, parsing configuration options
@author Donald Kline, Jr
*/


#include "PresConfig.h"
#include <iostream>
#include <fstream>

using namespace PresSim;

enum class PresState {correction, general, map, model};

/**
@fn
@brief Parses file into a PresConfig object
@param filename Name of file to parse
*/
PresConfig::PresConfig(std::string filename) : myConfig("PresSim"), myFaultMap(NULL_STR_PRES){

	//Step 1: Check if file is valid
	ifstream myfile(filename);
	if (!myfile.is_open())
	{
		PRES_ERROR("File "+filename+" not found");
		abort();
	}
	//Step 2: General parsing
	string line;
	PresState state = PresState::general;
	PresSubConfig psc(NULL_STR_PRES);
	bool in_progress = false;

	while ( getline (myfile, line) )
	{
		if (in_progress)
		{
			if (line.find("}")!=string::npos)
			{
				in_progress = false;
				switch (state) {
					case PresState::correction : 
						correction_schemes.push_back(std::move(psc));
						state = PresState::general;
						break;
					case PresState::model :
                        fault_models.push_back(std::move(psc));
                        state = PresState::general;
                        break;		
					case PresState::map :
						myFaultMap = std::move(psc);				
						state = PresState::general;
                        break;
					default: 
						PRES_ERROR("Parsing error- could not be parsed");
						abort();
				} 
			} else{
				if (line.find("=")!=string::npos)
				{
				    psc.parse_line(line);

				
				}
			}

		} else {
			if (line.find("CorrectionScheme") == 0 &&
				line.find(":")!=string::npos)
			{
				in_progress = true;
				state = PresState::correction;	
				PresSubConfig tmp(line.substr(line.find(":")+1));
                if (tmp.get_name()==NULL_STR_PRES)
                    continue; //No name defined
				psc = std::move(tmp);	

			} else if (line.find("FaultModel") == 0 &&
				line.find(":")!=string::npos)
			{
				in_progress = true;
				state = PresState::model;
				PresSubConfig tmp(line.substr(line.find(":")+1));
                if (tmp.get_name()==NULL_STR_PRES)
                    continue; //No name defined
                psc = std::move(tmp); 

			} else if (line.find("FaultMap") == 0 &&
				line.find(":")!=string::npos)
			{
				in_progress = true;
				state = PresState::map;
                PresSubConfig tmp(line.substr(line.find(":")+1));
           		if (tmp.get_name()==NULL_STR_PRES)
                	continue; //No name defined
                psc = std::move(tmp); 
			} else {
				if (line.find("=")!=string::npos)
					myConfig.parse_line(line);
				//TODO: add to log file to write?
			}


		}
		
	}	//end while		
	
} //end constructor
