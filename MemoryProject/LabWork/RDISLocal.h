#ifndef PRES_SIM_CORRECTION_SCHEME_RDISLOCAL_H
#define PRES_SIM_CORRECTION_SCHEME_RDISLOCAL_H
#include <iostream>
#include <vector>

class RDISLocal{

	public:
		    RDISLocal(const std::vector<uint8_t>& psc);
        void encode_data (uint64_t address, std::vector<uint8_t>& op, const std::vector<uint8_t>& old_data,
            const std::vector<uint32_t>& observed_faultybits, std::vector<uint8_t>& ps);
        uint32_t encode_dataless(uint64_t addr, uint32_t num_bits, const std::vector<uint32_t>& observed_faultybits,
            std::vector<uint8_t>& ps);
        void decode_data (uint64_t addr, std::vector<uint8_t>& op, std::vector<uint8_t>&ps);
        uint32_t decode_dataless (uint64_t addr, uint32_t num_bits, std::vector<uint32_t>&ps);
        uint32_t encoding_overhead (uint32_t data_size_in);
        void registerStatNames(bool (*stat_register)(const std::string s));
        void tick();


	protected:
		int granularity;
		bool protect_aux;
};

#endif
