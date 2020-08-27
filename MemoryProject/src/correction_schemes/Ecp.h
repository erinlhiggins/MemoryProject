#ifndef PRES_SIM_CORRECTION_SCHEME_ECP_H
#define PRES_SIM_CORRECTION_SCHEME_ECP_H
#include "../utils/pres_utils.h"

/**
@class Ecp : public PresSim::CorrectionScheme
@brief Definition for ECP (Error Correcting Pointers) 
<a href="https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/isca-ecp-final.pdf">ECP Paper</a> 
*/
class Ecp : public PresSim::CorrectionScheme{

	public:
		Ecp(const PresSim::PresSubConfig& psc);
        void encode_data (uint64_t addr, PresSim::OverheadPacket&, const std::vector<uint8_t>& old_data,
            const std::vector<uint32_t>& observed_faultybits, PresStatistics& ps);
        uint32_t encode_dataless(uint64_t addr, uint32_t num_bits, const std::vector<uint32_t>& observed_faultybits,
            PresStatistics& ps);
        void decode_data (uint64_t addr, PresSim::OverheadPacket&, PresStatistics& ps);
        uint32_t decode_dataless (uint64_t addr, uint32_t num_bits, PresStatistics& ps);
        uint32_t encoding_overhead (uint32_t data_size_in);
		uint32_t decoding_overhead (uint32_t data_and_aux_in);
        void registerStatNames(bool (*stat_register)(const string s));
        void tick();
	protected:
		uint32_t num_pointers;
		bool aux_protected;
		uint32_t encoding_ov_saved;
		uint32_t encoding_ov_input;
		uint32_t decoding_ov_input;
};

#endif
