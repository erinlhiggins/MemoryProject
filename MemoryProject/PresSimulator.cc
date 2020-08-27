#include "PresSim.h"
#include <iostream>
#include <cassert>
#include <cstring>
int main(int argc, char** argv){

	PresSim::ReliabilitySimulator rs("configs/ecp_fixed.txt");
	uint64_t v=0;
	std::vector<uint8_t> data, data_old, data_new;

	data.push_back(32); data.push_back(45);
	data_old.push_back(22); data_old.push_back(43);
	data_new.push_back(46); data_new.push_back(33);
	rs.tick();
	rs.tick();
	uint64_t tmp;
	tmp = rs.perform_read(0, data);
	assert(tmp!=UINT64_MAX);
	v+=tmp;
    tmp = rs.perform_write(0, data_old, data_new);
	assert(tmp!=UINT64_MAX);
	v+=tmp;	
	std::cout<<v<<endl;
}
