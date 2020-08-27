#include "RDISLocal.h"
#include <math.h>
#include <iostream>
#include <vector>

//TODO:
//possible issues: divisions not a perfect square, but that shouldn't be the case
//sigma and phi aren't intializing to 0, but also should not be the case

RDISLocal::RDISLocal(const std::vector<uint8_t>& psc){
 //nothing additional needed for this scheme
}

void encode_data (uint64_t address, const std::vector<uint8_t>& op, const std::vector<uint8_t>& old_data,
    const std::vector<uint32_t>& observed_faultybits, PresStatistics& ps){

//skipping the write and then read, assuming that the query function has updated results.

  bool fault0 = false;
  bool fault1 = false;

  //looking to see if the address being written to is a SA fault, if not, nothing needs to be done
  std::vector<uint32_t>* stuck_at_0_locations = {2};
  std::vector<uint32_t>* stuck_at_1_locations = {1,5};

  std::bool fault0 = false;
  std::bool fault1 = false;

  if(stuck_at_0_locations.size() > 0){
    fault0 = true;
  }

  if(stuck_at_1_locations.size() > 0){
    fault1 = true;
  }

  //if the address written to has no SA fault - do nothing and write like normal
  if(!fault0 && !fault1){
    //would write data here, just going to print instead
    cout << "No faults, data written: ";
    for (auto i = op.begin(); i != op.end(); ++i)
        cout << *i << " ";

    delete stuck_at_0_locations;
    delete stuck_at_1_locations;
    return;
  }

  //need to know how to divide up the various arrays to keep information about this set of bits
  std::int divisions = ceil((int)sqrt(op.size()));
  cout << "Divisions: ";
  cout << divisions;

  //if there is a fault, compute the VX and VY
  std::bool phi[divisions][dividions];
  std::bool sigma[divisions][divisions];
  std::bool VXK[divisions];
  std::bool VYK[divisions];
  std::vector<uint32_t> VX;
  std::vector<uint32_t> VY;
  std::int postition = 0;
  std::bool breakCondition = false;
  std::int limitK = 0;

  //TODO: validate this, i need a way to look at each bit
  //creating c(i,j) 8 bits to be stored, turning that into a 4x4 matix of bits
  std::int c[divisions][divisions];
  for(int l = 0; l < divisions; l++){
    for(int m = 0; m < divisions; m++){
      //padding with 0 if not an even square
      c[l][m] = (if position < op.size) ? op[postition] : 0;
      position++;
    }
  }

  //iterate through the stuck at vectors and use op.getbitpotiion(i) to see if the values are stuck at wrong or right
  for(int s = 0; s < stuck_at_0_locations.size(); s++){
    std::int row = s / divisions;
    std:: int col = s % divisions;
    if(op.[stuck_at_0_locations[s]] == 0){
      phi[row][col] = 1;
      sigma[row][col] = 0;
    }else{
      phi[row][col] = 1;
      sigma[row][col] = 1;
    }
  }
  for(int t = 0; t < stuck_at_1_locations.size(); t++){
    std::int row2 = t / divisions;
    std:: int col2 = t % divisions;
    if(op[stuck_at_1_locations[t]] == 1){
      phi[row2][col2] = 1;
      sigma[row2][col2] = 0;
    }else{
      phi[row2][col2] = 1;
      sigma[row2][col2] = 1;
    }
  }


  //compute VX and VY
  //limit of K set to 10, can be changed
  while(!breakCondition && limitK < 10){
    for(int x = 0 x < divisions; x++){
      VXK[x] = sigma[x][0] || sigma[x][1] || sigma[x][2] || sigma[x][3];
      VYK[x] = sigma[0][x] || sigma[1][x] || sigma[2][x] || sigma[3][x];
      if(VXK[x]){
        VX[x] = VX[x] + 1;
      }
      if(VYK[x]){
        VY[x] = VY[x] + 1;
      }
    }
    for(x = 0; x < divisions; x++){
      for(y = 0; y < divisions; y++){
        if(!VXK[x] || !VYK[y]){
          phi[x][y] = 0;
          sigma[x][y] = 0;
        }else if(phi[x][y]){
          (if sigma[x][y] == 1) ? 0 : 1;
        }
      }
    }
    for(int y = 0; y < divisions; y++){
      if(VXK[y] || VYK[y]){
        breakCondition = true;
        break;
      }
    }
    limitK++;
  }

  //inverting bits if need be and writing them according to flips or not
  std::int currLoc = 0;
  for(int q = 0; q < divisions; q++){
    for(int w = 0; w < divisions; w++){
      if(std::min(VX[q], VY[w]) % 2 != 0){
        (if op[currLoc] == 0) ? op[currLoc] = 1 : op[currLoc] = 0;
      }
      currLoc++;
    }
  }

  //would write here, but instead just going to print
  cout << "Final version of data written: ";
  for (auto j = op.begin(); j != op.end(); ++j)
      cout << *j << " ";

  //delete information not needed
  delete stuck_at_0_locations;
  delete stuck_at_1_locations;
  return;

}

uint32_t encode_dataless(uint64_t addr, uint32_t num_bits, const std::vector<uint32_t>& observed_faultybits,
    PresStatistics& ps){
      std::int returnValue = 0;
      std::vector<uint32_t>* stuck_at_0_locations = queryWeaknessesFromChild(0, address, 0, op.data_bits.size()*8);
      std::vector<uint32_t>* stuck_at_1_locations = queryWeaknessesFromChild(1, address, 0, op.data_bits.size()*8);
      if(data_size_in == 512){
        if(21 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
          returnValue = std::abs(21 - (stuck_at_0_locations + stuck_at_1_locations));
        }
      }else if(data_size_in == 1024){
        if(26 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
          returnValue = std::abs(26 - (stuck_at_0_locations + stuck_at_1_locations));
        }
      }else if(data_size_in == 2048){
        if(35 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
          returnValue = std::abs(35 - (stuck_at_0_locations + stuck_at_1_locations));
        }
      }else if(data_size_in == 4096){
        if(45 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
          returnValue = std::abs(45 - (stuck_at_0_locations + stuck_at_1_locations));
        }
      }else if(data_size_in == 8192){
        if(60 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
          returnValue = std::abs(60 - (stuck_at_0_locations + stuck_at_1_locations));
        }
      }else{
        if(data_size_in < 512){
          if(80 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
            returnValue = std::abs(80 - (stuck_at_0_locations + stuck_at_1_locations));
          }
        }else if(data_size_in > 8192){
          if(15 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
            returnValue = std::abs(15 - (stuck_at_0_locations + stuck_at_1_locations));
          }
        }
      }
      delete stuck_at_0_locations;
      delete stuck_at_1_locations;
      return returnValue;
}

void decode_data (uint64_t address, PresSim::OverheadPacket& op, PresStatistics& ps){

  //need to know how to divide up the various arrays to keep information about this set of bits
  std::int divisions = ceil((int)sqrt(op.data_bits.size()*8));

  //generating everything as when doing reads/writes
  std::bool phi[divisions][dividions];
  std::bool sigma[divisions][divisions];
  std::bool VXK[divisions];
  std::bool VYK[divisions];
  std::vector<uint32_t> VX;
  std::vector<uint32_t> VY;
  std::int postition = 0;
  std::bool breakCondition = false;
  std::int limitK = 0;

  //TODO: validate this, i need a way to look at each bit
  //creating c(i,j) 8 bits to be stored, turning that into a 4x4 matix of bits
  std::int c[divisions][divisions];
  for(int l = 0; l < divisions; l++){
    for(int m = 0; m < divisions; m++){
      c[l][m] = op.getbit(postition);
      position++;
    }
  }

  //iterate through the stuck at vectors and use op.getbitpotiion(i) to see if the values are stuck at wrong or right
  for(int s = 0; s < stuck_at_0_locations.size(); s++){
    std::int row = s / divisions;
    std:: int col = s % divisions;
    if(op.getbit(stuck_at_0_locations[s]) == 0){
      phi[row][col] = 1;
      sigma[row][col] = 0;
    }else{
      phi[row][col] = 1;
      sigma[row][col] = 1;
    }
  }
  for(int t = 0; t < stuck_at_1_locations.size(); t++){
    std::int row2 = t / divisions;
    std:: int col2 = t % divisions;
    if(op.getbit(stuck_at_1_locations[t]) == 1){
      phi[row2][col2] = 1;
      sigma[row2][col2] = 0;
    }else{
      phi[row2][col2] = 1;
      sigma[row2][col2] = 1;
    }
  }


  //compute VX and VY
  //limit of K set to 10, can be changed
  while(!breakCondition && limitK < 10){
    for(int x = 0 x < divisions; x++){
      VXK[x] = sigma[x][0] || sigma[x][1] || sigma[x][2] || sigma[x][3];
      VYK[x] = sigma[0][x] || sigma[1][x] || sigma[2][x] || sigma[3][x];
      if(VXK[x]){
        VX[x] = VX[x] + 1;
      }
      if(VYK[x]){
        VY[x] = VY[x] + 1;
      }
    }
    for(x = 0; x < divisions; x++){
      for(y = 0; y < divisions; y++){
        if(!VXK[x] || !VYK[y]){
          phi[x][y] = 0;
          sigma[x][y] = 0;
        }else if(phi[x][y]){
          (if sigma[x][y] == 1) ? 0 : 1;
        }
      }
    }
    for(int y = 0; y < divisions; y++){
      if(VXK[y] || VYK[y]){
        breakCondition = true;
        break;
      }
    }
    limitK++;
  }

  //read, flipping the bits accordingly
  std::int currLoc = 0;
  for(int q = 0; q < divisions; q++){
    for(int w = 0; w < divisions; w++){
      if(std::min(VX[q], VY[w]) % 2 != 0){
        (if op.getbit(currLoc) == 0) ? op.setbit(1) : op.setbit(0);
      }
      currLoc++;
    }
  }
  op.readaux(address, ceil(log2(8*op.data_bits.size())));
}

//same as encoding
uint32_t decode_dataless (uint64_t addr, uint32_t num_bits, PresStatistics& ps){
  std::int returnValue = 0;
  std::vector<uint32_t>* stuck_at_0_locations = queryWeaknessesFromChild(0, address, 0, op.data_bits.size()*8);
  std::vector<uint32_t>* stuck_at_1_locations = queryWeaknessesFromChild(1, address, 0, op.data_bits.size()*8);
  if(data_size_in == 512){
    if(21 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
      returnValue = std::abs(21 - (stuck_at_0_locations + stuck_at_1_locations));
    }
  }else if(data_size_in == 1024){
    if(26 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
      returnValue = std::abs(26 - (stuck_at_0_locations + stuck_at_1_locations));
    }
  }else if(data_size_in == 2048){
    if(35 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
      returnValue = std::abs(35 - (stuck_at_0_locations + stuck_at_1_locations));
    }
  }else if(data_size_in == 4096){
    if(45 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
      returnValue = std::abs(45 - (stuck_at_0_locations + stuck_at_1_locations));
    }
  }else if(data_size_in == 8192){
    if(60 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
      returnValue = std::abs(60 - (stuck_at_0_locations + stuck_at_1_locations));
    }
  }else{
    if(data_size_in < 512){
      if(80 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
        returnValue = std::abs(80 - (stuck_at_0_locations + stuck_at_1_locations));
      }
    }else if(data_size_in > 8192){
      if(15 - (stuck_at_0_locations + stuck_at_1_locations) < 0){
        returnValue = std::abs(15 - (stuck_at_0_locations + stuck_at_1_locations));
      }
    }
  }
  delete stuck_at_0_locations;
  delete stuck_at_1_locations;
  return returnValue;
}

uint32_t encoding_overhead (uint32_t data_size_in){
  if(data_size_in == 512){
    return 512*0.19;
  }else if(data_size_in == 1024){
    return 1024 * 0.12;
  }else if(data_size_in == 2048){
    return 2048 * 0.09;
  }else if(data_size_in == 4096){
    return 4096 * 0.06;
  }else if(data_size_in == 8192){
    return 8192 * 0.04;
  }else{
    if(data_size_in < 512){
      return data_size_in * 0.20;
    }else if(data_size_in > 8192){
      return data_size_in * 0.02;
    }
  }
}

//no additional information is needed to decode
uint32_t Ecp::decoding_overhead (uint32_t data_and_aux_size){
  return 0;
}

//not used in this case yet
void registerStatNames(bool (*stat_register)(const string s)){
  return();
}

//not needed for this case
void tick(){
  return();
}
