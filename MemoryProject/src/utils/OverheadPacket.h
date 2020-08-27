/**
@file
@brief OverheadPacket definitions
@author Donald Kline, Jr
*/
#ifndef PRES_SIM_OVERHEAD_PACKET_H
#define PRES_SIM_OVERHEAD_PACKET_H

#include "GenUtils.h"

namespace PresSim{

/**
@class OverheadPacket
@brief Class to keep track of data changes, and area/timing/power overheads
*/
class OverheadPacket{

    public:
        OverheadPacket(int data_size, int max_size);
		OverheadPacket(OverheadPacket& rhs);    
		bool getbit(uint32_t position);
		void setbit(uint32_t position, bool isOne);

		bool writeaux(bool val);
		bool writeaux(uint32_t val, uint32_t numbits);
		bool readaux(bool& val);
		bool readaux(uint32_t& val, uint32_t numbits);

		bool addbaseaux(uint32_t bits);
		bool subtractbaseaux(uint32_t bits);
		uint32_t current_overhead(){return data_bits.size()*8+aux_base_position;};
		void print_data() const;
        void add_delay(uint64_t time) {time_ps+=time;}
        void add_dyn_energy(uint64_t dynamic_energy) {dyn_energy_fJ+=dynamic_energy;}
	    void add_static_power(uint64_t static_power) {static_power_fW+=static_power;}
		//directly accessable
        std::vector<uint8_t> data_bits;
        std::vector<bool> aux_bits;

		std::vector<uint64_t> get_stats();

    protected:

		uint64_t area_nm2;
        uint64_t time_ps;
        uint64_t dyn_energy_fJ;
		uint64_t static_power_fW;

	private:
		uint32_t aux_base_position;
		uint32_t aux_relative_position;

};

} //end PresSim namespace
#endif
