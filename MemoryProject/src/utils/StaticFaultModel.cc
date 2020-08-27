#include "StaticFaultModel.h"

namespace PresSim{
StaticFaultModel::StaticFaultModel(const PresSubConfig& ps) : FaultModel(ps) 
{

	string filename;
	if (!ps.get_value(filename, "static_fault_file"))
    {
        PRES_ERROR("Missing StaticFaultModel parameter: static_fault_file");
        abort();
    }	

	get_faults_size(filename);
	read_faults(filename);

}

StaticFaultModel::~StaticFaultModel() {
	
	    for (int tmp = 0; tmp<m_rows; tmp++)
        {
        	delete(m_faults[tmp]);
        }
		delete(m_faults);
}

void StaticFaultModel::get_faults_size(string filename){

        ifstream f;
        f.open(filename);

        if (!f.is_open())
        {
                PRES_ERROR("ERROR: File "<<filename<<" entered for StaticFaultModel does not exist!");
                abort();
        }
        int line_num = 0;
        std::string line;
        while (std::getline(f, line))
        {
                if (line.find("#")==0)
                {
                        if (line.find("total_rows")!=std::string::npos)
                        {
                                m_rows= atoi(line.substr(line.find(":")+1).c_str());
                        }
                        else if (line.find("max_column")!=std::string::npos)
                        {
                                m_cols = atoi(line.substr(line.find(":")+1).c_str());
                        }
                } else if (line_num>10)
                {
                        break;
                }
                line_num++;
        }
    f.close();
}


void StaticFaultModel::read_faults(string filename){
        ifstream f;
        f.open(filename);
        if (!f.is_open())
        {
            PRES_ERROR("ERROR: File "<<filename<<" entered for StaticFaultModel does not exist!");    
			abort();
        }
		m_faults = new bool*[m_rows];
		for (int tmp = 0; tmp<m_rows; tmp++)
		{
			m_faults[tmp] = new bool[m_cols];
			memset(m_faults[tmp], false, sizeof(bool)*m_cols);
		}

        int cur_num = 0;
        std::string line;
        while (std::getline(f, line))
        {
                if (line.find("#") != 0)
                {
                        int offset = line.find(" ");
                        std::string row_str = line.substr(0, offset);
                        std::string col_str = line.substr(offset + 1);

                        int row = atoi(row_str.c_str());
                        int col = atoi(col_str.c_str());

						if (row<m_rows && col<m_cols)
							m_faults[row][col] = true;
				}
        }
        f.close();
}

}//end namespace
