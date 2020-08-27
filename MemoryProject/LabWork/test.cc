//test file for Erin's additionsdividions
#include <iostream>
#include <vector>
#include <math.h>

int main(){
  //test 1 - regular 16 bit write to this data location
  //data location:
  //data content: 0000000000000000
  std::vector<uint8_t> newInfo;
  newInfo.push_back(1);
  newInfo.push_back(1);
  newInfo.push_back(0);
  newInfo.push_back(1);
  newInfo.push_back(1);
  newInfo.push_back(0);
  newInfo.push_back(0);
  newInfo.push_back(1);
  newInfo.push_back(0);
  newInfo.push_back(1);
  newInfo.push_back(1);
  newInfo.push_back(0);
  newInfo.push_back(1);
  newInfo.push_back(0);
  newInfo.push_back(0);
  newInfo.push_back(1);
  std::vector<uint8_t> oldInfo;
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);
  oldInfo.push_back(1);

  //looking to see if the address being written to is a SA fault, if not, nothing needs to be done
  std::vector<uint32_t> stuck_at_0_locations;
  std::vector<uint32_t> stuck_at_1_locations;

  bool fault0 = false;
  bool fault1 = false;

  if(stuck_at_0_locations.size() > 0){
    fault0 = true;
  }

  if(stuck_at_1_locations.size() > 0){
    fault1 = true;
  }

  //if the address written to has no SA fault - do nothing and write like normal
  if(!fault0 && !fault1){
    //would write data here, just going to print instead
    std::cout << "No faults, data written: ";
    for (auto i = newInfo.begin(); i != newInfo.end(); ++i)
        std::cout << *i << " ";

  }

  //need to know how to divide up the various arrays to keep information about this set of bits
  int divisions = ceil((int)sqrt(newInfo.size()));
  std::cout << "Divisions: ";
  std::cout << divisions;

  //if there is a fault, compute the VX and VY
  bool phi[divisions][divisions];
  bool sigma[divisions][divisions];
  bool VXK[divisions];
  bool VYK[divisions];
  std::vector<uint32_t> VX;
  std::vector<uint32_t> VY;
  int position = 0;
  bool breakCondition = false;
  int limitK = 0;

  //TODO: validate this, i need a way to look at each bit
  //creating c(i,j) 8 bits to be stored, turning that into a 4x4 matix of bits
  int c[divisions][divisions];
  for(int l = 0; l < divisions; l++){
    for(int m = 0; m < divisions; m++){
      //padding with 0 if not an even square
      c[l][m] = (position < newInfo.size()) ? newInfo[position] : 0;
      position++;
    }
  }

  //iterate through the stuck at vectors and use op.getbitpotiion(i) to see if the values are stuck at wrong or right
  for(int s = 0; s < stuck_at_0_locations.size(); s++){
    int row = s / divisions;
    int col = s % divisions;
    if(newInfo[stuck_at_0_locations[s]] == 0){
      phi[row][col] = 1;
      sigma[row][col] = 0;
    }else{
      phi[row][col] = 1;
      sigma[row][col] = 1;
    }
  }
  for(int t = 0; t < stuck_at_1_locations.size(); t++){
    int row2 = t / divisions;
    int col2 = t % divisions;
    if(newInfo[stuck_at_1_locations[t]] == 1){
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
    for(int x = 0; x < divisions; x++){
      VXK[x] = sigma[x][0] || sigma[x][1] || sigma[x][2] || sigma[x][3];
      VYK[x] = sigma[0][x] || sigma[1][x] || sigma[2][x] || sigma[3][x];
      if(VXK[x]){
        VX[x] = VX[x] + 1;
      }
      if(VYK[x]){
        VY[x] = VY[x] + 1;
      }
    }
    for(int l = 0; l < divisions; l++){
      for(int m = 0; m < divisions; m++){
        if(!VXK[l] || !VYK[m]){
          phi[l][m] = 0;
          sigma[l][m] = 0;
        }else if(phi[l][m]){
          (sigma[l][m] == 1) ? 0 : 1;
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
  int currLoc = 0;
  for(int q = 0; q < divisions; q++){
    for(int w = 0; w < divisions; w++){
      int lesser = (VX[q] > VY[w]) ? VY[w] : VX[q];
      if(lesser % 2 != 0){
        (newInfo[currLoc] == 0) ? newInfo[currLoc] = 1 : newInfo[currLoc] = 0;
      }
      currLoc++;
    }
  }

  //would write here, but instead just going to print
  std::cout << "Final version of data written: ";
  for (auto j = newInfo.begin(); j != newInfo.end(); ++j)
      std::cout << *j << " ";



  //test 2 - 18 bit write
  //data location:
  //data content: 000000000000000000

  //test 3 - 16 bit write with 1 SA-W
  //data location:
  //data content: 0000000000000000

  //test 4 - 16 bit write with 1 SA-R
  //data location:
  //data content: 0000000000000000

  //test 5 - 16 bit write with 1 SA-R and 1 SA-W
  //data location:
  //data content: 0000000000000000

  //test 6 - 16 bit write with 2 SA-W
  //data location:
  //data content: 0000000000000000

  //test 7 - 16 bit write with 2 SA-R
  //data location:
  //data content: 0000000000000000

  //test 8 - 16 bit write with 2 SA-W and 2 SA-R
  //data location:
  //data content: 0000000000000000

  //test 1 - regular 16 bit write to this data location
  //data location:
  //data content: 1111111111111111

  //test 2 - 18 bit write
  //data location:
  //data content: 111111111111111111

  //test 3 - 16 bit write with 1 SA-W
  //data location:
  //data content: 1111111111111111

  //test 4 - 16 bit write with 1 SA-R
  //data location:
  //data content: 1111111111111111

  //test 5 - 16 bit write with 1 SA-R and 1 SA-W
  //data location:
  //data content: 1111111111111111

  //test 6 - 16 bit write with 2 SA-W
  //data location:
  //data content: 1111111111111111

  //test 7 - 16 bit write with 2 SA-R
  //data location:
  //data content: 1111111111111111

  //test 8 - 16 bit write with 2 SA-W and 2 SA-R
  //data location:
  //data content: 1111111111111111

  //test 1 - regular 16 bit write to this data location
  //data location:
  //data content: 1010101010101010

  //test 2 - 18 bit write
  //data location:
  //data content: 101010101010101010

  //test 3 - 16 bit write with 1 SA-W
  //data location:
  //data content: 1010101010101010

  //test 4 - 16 bit write with 1 SA-R
  //data location:
  //data content: 1010101010101010

  //test 5 - 16 bit write with 1 SA-R and 1 SA-W
  //data location:
  //data content: 1010101010101010

  //test 6 - 16 bit write with 2 SA-W
  //data location:
  //data content: 1010101010101010

  //test 7 - 16 bit write with 2 SA-R
  //data location:
  //data content: 1010101010101010

  //test 8 - 16 bit write with 2 SA-W and 2 SA-R
  //data location:
  //data content: 1010101010101010

}
