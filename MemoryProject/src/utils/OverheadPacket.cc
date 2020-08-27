#include "OverheadPacket.h"

using std::endl;
using std::cout;

using namespace PresSim;

OverheadPacket::OverheadPacket(int d, int c):
	data_bits(d, 0),
	aux_bits(c, 0),
	area_nm2(0),
	time_ps(0),
	dyn_energy_fJ(0),
	static_power_fW(0),
	aux_base_position(0),
  	aux_relative_position(0)
{
	

}

OverheadPacket::OverheadPacket(OverheadPacket& rhs){

	this->time_ps = rhs.time_ps;
	this->dyn_energy_fJ = rhs.dyn_energy_fJ;
	this->static_power_fW = rhs.static_power_fW;
	this->area_nm2 = rhs.area_nm2;

	this->data_bits = rhs.data_bits;
	this->aux_bits = rhs.aux_bits;
}

void OverheadPacket::print_data() const{

	for (int i=0; i<data_bits.size()-1;i++)
	{
		cout<<(uint32_t)data_bits[i]<<",";
	}
	cout<<(uint32_t)data_bits[data_bits.size()-1]<<"----";

	for (int i=0; i<aux_bits.size(); i++)
		cout<<(aux_bits[i]?1:0) <<",";
	cout<<endl;


}
bool OverheadPacket::getbit(uint32_t position){

	if (position<data_bits.size()*8)
	{
		uint8_t tmp = position/8;
		uint8_t tmp2 = position%8;

		return data_bits[tmp]&(1<<tmp2);
	}
	else if (position-data_bits.size()*8 < aux_bits.size()){
		return aux_bits[position-data_bits.size()*8];
	}else{
		PRES_ERROR("INDEX to OverheadPacket::gitbit out of range->"+position);
		abort();
	}
}

void OverheadPacket::setbit(uint32_t position, bool isOne){
    if (position<data_bits.size()*8)
    {
        uint8_t tmp = position/8;
        uint8_t tmp2 = position%8;

		if (isOne)	
        	data_bits[tmp] |= (1<<tmp2);
		else
			data_bits[tmp] &= ~(1<<tmp2);
    }
    else if (position-data_bits.size()*8 < aux_bits.size()){
        aux_bits[position-data_bits.size()*8]=isOne;
    }else{
        PRES_ERROR("INDEX to OverheadPacket::setbit out of range->"+position);
        abort();
    }
}

bool OverheadPacket::writeaux(bool val){
		//TODO check to make sure aux_relative_position is valid
		//cout<<"Writing in position "<<aux_relative_position<<endl;
		aux_bits[aux_relative_position] = val;
		aux_relative_position++;
		//cout<<"\tWrote "<<val<<endl;
		return true;
}
    
bool OverheadPacket::writeaux(uint32_t val, uint32_t numbits){	
	//cout<<"writing in positions "<<aux_relative_position<<" to "<<(aux_relative_position+numbits)<<endl;
	uint32_t n = val;
	std::vector<bool> go;
	//STEP1: convert to binary
	for (int i=0; n>0; i++)
	{
		if(i>numbits) //can't be represented in size numbits
			return false;
		go.push_back(n%2==1);
		n=n/2;
	}
	if (val==0)
		go.push_back(false);

	//STEP2: write
	for (int i=0; i<numbits-go.size();i++)
	{
		aux_bits[aux_relative_position] = false;
		aux_relative_position++;
	}
	for (int i=go.size()-1; i>=0; i--)
	{
		aux_bits[aux_relative_position]=go[i];
		aux_relative_position++;
	}
	//cout<<"\tWrote "<<val<<endl;
	//cout<<"\tAux relative is now "<<aux_relative_position<<endl;
	return true;
	


}
  
bool OverheadPacket::readaux(bool& val){
	//TODO check to make sure aux_relative_position is valid
	//cout<<"Reading in position "<<aux_relative_position<<endl;
	val = aux_bits[aux_relative_position];
	aux_relative_position++;
	//cout<<"\tRead "<<val<<endl;
	return true;
}

bool OverheadPacket::readaux(uint32_t& val, uint32_t numbits){
	//cout<<"Reading in positions "<<aux_relative_position<<" to " <<(aux_relative_position+numbits)<<endl;
	uint32_t dec_value = 0;
	uint32_t pos = aux_relative_position+numbits-1;

	int shift = 0;
	for (; pos>=aux_relative_position; pos--)
	{
		if (aux_bits[pos])
			dec_value+=1<<shift;
		shift++;
	}		
	val = dec_value;
	//cout<<"\tRead "<<val<<endl;
	aux_relative_position+=numbits;
	return true;
}

bool OverheadPacket::addbaseaux(uint32_t bits){
	//TODO check to make sure no overflow
	aux_base_position+=bits;
	aux_relative_position=aux_base_position;
	return true;
}
        
bool OverheadPacket::subtractbaseaux(uint32_t bits){
		if (bits>aux_base_position)
			return false;
        aux_base_position-=bits;
		aux_relative_position = aux_base_position;
		return true;
}

std::vector<uint64_t> OverheadPacket::get_stats(){

		std::vector<uint64_t> to_return(4);
		to_return[0]=time_ps;
		to_return[1]=dyn_energy_fJ;
		to_return[2]=static_power_fW;
		to_return[3]=area_nm2;

		return to_return;
}

