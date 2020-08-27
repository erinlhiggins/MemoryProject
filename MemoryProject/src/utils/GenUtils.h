/**
@file
@brief General file for common includes and macros
@author Donald Kline, Jr
*/

#ifndef __PRES_GEN_UTILS_H__
#define __PRES_GEN_UTILS_H__

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <unordered_set>
#include <fstream>
#include <string>
#include <functional>
#define PRES_ERROR(str) std::cerr<<"[PRES ERROR ("<<__FILE__<<":"<<__LINE__<<")]: "<<str<<std::endl
#define NULL_STR_PRES "__PRES_STRING_IS_NULL_NULL_IS_PRES_STRING__"

using std::string;
using std::map;
using std::unordered_set;

/*
 * @fn bitCount(T c)
 * @brief Counts the number of set bits in any primitive type. O(bits set) instead of O(n).
 * @param c The value to be counted
 * @return uint8_t representing how many of the bits are set in the input
 */
template <class T>
uint8_t bitCount(T c) {
    static_assert(std::is_arithmetic<T>::value, "error message");
    uint8_t count = 0;
    while (c!=0)
    {
        c = c&(c-1);
        count++;
    }
    return count;
}

/*
 * @fn bitFlipCount(const T* a, const T* b, int sz)
 * @brief Counts the number of flips to '0' and '1' from a to b
 * @param a 'Old' values
 * @param b 'New' values
 * @param sz Size of array
 * @return tule<uint32_t,uint32_t> total number of flips from a to b <from 1 to 0, from 0 to 1>
 */
template <class T>
std::tuple<uint32_t,uint32_t> bitFlipCount(const T* a, const T* b, int sz)
{
    static_assert(sizeof(T)<=sizeof(uint64_t), "Operator must be of an equal or larger arithmetic type!");
    static_assert(std::is_arithmetic<T>::value, "bitFlipCount is only valid with arithmetic types");

    uint32_t count0 = 0; //FLIPS from 1 (a) to 0 (b)
    uint32_t count1 = 0; //FLIPS from 0 (a) to 1 (b)
    uint32_t s_elems = (sizeof(T)*sz)/sizeof(uint64_t);

    //For optimization, do both bit-level ops and counting at higher granularity
    for (int i=0; i<s_elems; i++)
    {
        count0+= bitCount(((uint64_t*)a)[i] & ~(((uint64_t*)b)[i]));
        count1+= bitCount(~(((uint64_t*)a)[i]) & (((uint64_t*)b)[i]));
    }
    //If the number of T elements did not fit evenly into uint64_t, do rest at T granularity
    uint32_t completed = (s_elems*sizeof(uint64_t))/(sizeof(T));
    for (int i=completed; i<sz; i++)
    {
        count0+= bitCount((a[i]) & ~(b[i]));
        count1+= bitCount(~(a[i]) & (b[i]));

    }
    return std::make_tuple(count0, count1);
}

class PresStatistics{

	public:
		PresStatistics() {cached_label = NULL_STR_PRES;};
		bool addRootStat(const string name) {
			if (m_root_stats.count(name)>0)
				return false;
			m_root_stats[name] = 0;
			return true;
		}
		bool addCorrectionStat(const string id, const string name) {
			if (m_correction_stats.count(id+"::::"+name)>0)
				return false;
			m_correction_stats[id+"::::"+name] = 0;
			if (m_correction_ids.count(id)==0)
				m_correction_ids.insert(id);
			return true;
		}
		bool addCorrectionStat(const string name) {
			if(cached_label == NULL_STR_PRES)
                return false;
            return addCorrectionStat(cached_label, name);

		}
		bool addModelStat(const string id, const string name) {
			if (m_model_stats.count(id+"::::"+name)>0)
                return false;
            m_model_stats[id+"::::"+name] = 0;
			if (m_model_ids.count(id)==0)
                m_model_ids.insert(id);
            return true;
		}

        bool addModelStat(const string name) {
            if(cached_label == NULL_STR_PRES)
                return false;
            return addModelStat(cached_label, name);        

		}

		bool setCachedLabel(const string s){
			if (m_correction_ids.count(s)<1 
				&& m_model_ids.count(s)<1)
				return false;
			cached_label = s;
			return true;
		}

		bool updateRootStat(const string name, uint64_t value){
			if (m_root_stats.count(name)<1)
				return false;
			m_root_stats[name]+=value;
			return true;
		}

		bool updateCorrectionStat(const string id, const string name, uint64_t value)
        {
            if(m_correction_stats.count(id+"::::"+name)<1)
                return false;
            m_correction_stats[id+"::::"+name] += value;
            return true;
        }
		bool updateCorrectionStat(const string name, uint64_t value)
		{
			if(cached_label == NULL_STR_PRES)
				return false;
			return updateCorrectionStat(cached_label, name, value);
		}
		
        bool updateModelStat(const string id, const string name, uint64_t value)
        {
            if(m_model_stats.count(id+"::::"+name)<1)
                return false;
            m_model_stats[id+"::::"+name] += value;
            return true;
        }
        bool updateModelStat(const string name, uint64_t value)
        {
            if(cached_label == NULL_STR_PRES)
                return false;
			return updateModelStat(cached_label, name, value);

        }

		void printStats(std::ostream& stream){
			stream<<"Begin PresSim Statistics.\n";
			stream<<"Global Stats\n";
			for (auto it = m_root_stats.begin(); it!=m_root_stats.end(); ++it)
			{
				stream<<"\t"<< it->first << ": "<< it->second << "\n";
			}	
			stream<< "Correction Stats\n";
			for (auto it = m_correction_ids.begin(); it!=m_correction_ids.end(); ++it)
			{
				stream<<"\t"<< (*it)<<"\n";
				for (auto it2 = m_correction_stats.begin(); it2!=m_correction_stats.end(); ++it2)
				{
					if (it2->first.find(*it) == 0) //if it starts with the current id
					{
						stream<<"\t\t"<<it2->first.substr((*it).size()+4);
					}
				}
			}
		    stream<<"Model Stats\n";
            for (auto it = m_model_ids.begin(); it!=m_model_ids.end(); ++it)
            {
                stream<<"\t"<< (*it)<<"\n";
                for (auto it2 = m_model_stats.begin(); it2!=m_model_stats.end(); ++it2)
                {
                    if (it2->first.find(*it) == 0) //if it starts with the current id
                    {
                        stream<<"\t\t"<<it2->first.substr((*it).size()+4);
                    }
                }
            }
			stream<<"End PresSim Statistics.\n";	
					

		}

	private:
		map<string, uint64_t> m_root_stats;
		unordered_set<string> m_correction_ids;
		map<string, uint64_t> m_correction_stats;
		unordered_set<string> m_model_ids;
		map<string, uint64_t> m_model_stats;
		string cached_label;
};

#endif
