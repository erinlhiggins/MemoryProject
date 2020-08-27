/**
@file
@brief Basic3f fault map implementation (toy example)
*/

#include "Basic3f_fmap.h"

Basic3f_fmap::Basic3f_fmap(const PresSim::PresSubConfig& psc) : PresSim::FaultMap(psc) {
	//do nothing
}


void Basic3f_fmap::add_fault(uint64_t address, uint32_t bit_offset, int id)
{
//Do nothing 
}
		
bool Basic3f_fmap::isVulnerable(uint64_t address, PresSim::OverheadPacket& data, uint32_t position){

	//TODO
	return false;
}

std::vector<uint32_t>* Basic3f_fmap::isVulnerableRange(uint64_t address, PresSim::OverheadPacket& data, uint32_t start, uint32_t end)
{
	std::vector<uint32_t> tmp;
	//TODO
	return &tmp;

}


bool Basic3f_fmap::isWeak(uint64_t address, uint32_t position){
	//TODO
	return false;

}


std::vector<uint32_t>* Basic3f_fmap::isWeakRange(uint64_t address,  uint32_t start, uint32_t end){
	std::vector<uint32_t> tmp;

	return &tmp;
	//TODO
}


std::vector<unsigned int> Basic3f_fmap::get_faults(uint64_t address) const
{
	std::vector<unsigned int>  to_return(3);
	to_return.push_back(2);
	to_return.push_back(5);
	to_return.push_back(21);
	
	return to_return;
}

