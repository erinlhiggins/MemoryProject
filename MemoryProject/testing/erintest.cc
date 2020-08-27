#include <stdio.h>
#include <math.h>

int main(){
  //test 1 - regular 16 bit write to this data location
  //data location:
  //data content: 0000000000000000
  int newInfo[] = {1,1,1,1,1,0,0,1,0,1,1,0,0,0,0,0};

  int stuck_at_0_locations[] = {1,15};
  int stuck_at_1_locations[] = {3,4, 5, 7, 8};

  int divisions = ceil((int)sqrt(16));
  printf("Divisions: %d \n", divisions);

  //if there is a fault, compute the VX and VY
  bool phi[4][4] = {{false,false,false,false},{false,false,false,false}};
  bool sigma[4][4] = {{false,false,false,false},{false,false,false,false}};
  bool VXK[4] = {false,false,false,false};
  bool VYK[4] = {false,false,false,false};
  int VX[4] = {false,false,false,false};
  int VY[4] = {false,false,false,false};
  int position = 0;
  bool breakCondition = false;
  int limitK = 0;

  int c[divisions][divisions];
  for(int l = 0; l < divisions; l++){
    for(int m = 0; m < divisions; m++){
      //padding with 0 if not an even square
      c[l][m] = (position < 16) ? newInfo[position] : 0;
      position++;
      printf("%d", c[l][m]);
    }
    printf("\n");
  }

  printf("\n");
  for(int s = 0; s < 2; s++){
    int row = stuck_at_0_locations[s] / divisions;
    int col = stuck_at_0_locations[s] % divisions;
    printf("\n");
    printf("new info location: %d", newInfo[stuck_at_0_locations[s]]);
    printf("\n");
    if(newInfo[stuck_at_0_locations[s]] == 0){
      phi[row][col] = 1;
      sigma[row][col] = 0;
    }else{
      phi[row][col] = 1;
      sigma[row][col] = 1;
    }
  }
  for(int t = 0; t < 2; t++){
    int row2 = stuck_at_1_locations[t] / divisions;
    int col2 = stuck_at_1_locations[t] % divisions;
    if(newInfo[stuck_at_1_locations[t]] == 1){
      phi[row2][col2] = 1;
      sigma[row2][col2] = 0;
    }else{
      phi[row2][col2] = 1;
      sigma[row2][col2] = 1;
    }
  }

  for(int l = 0; l < divisions; l++){
    for(int m = 0; m < divisions; m++){
      printf("%d", phi[l][m]);
    }
    printf("\n");
  }

  printf("\n\n");
  for(int l = 0; l < divisions; l++){
    for(int m = 0; m < divisions; m++){
      printf("%d", sigma[l][m]);
    }
    printf("\n");
  }

  printf("\n\n");
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

  printf("Limit k: %d \n", limitK);

  printf("VX: ");
  for (int j = 0; j < 4; ++j)
      printf("%d ", VX[j]);

  printf("\n");

  printf("VY: ");
  for (int h = 0; h < 4; ++h)
      printf("%d ", VY[h]);

  printf("\n");
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
  printf("Final version of data written: ");
  for (int j = 0; j < 16; ++j)
      printf("%d ", newInfo[j]);

  int currLoc2 = 0;
  for(int q = 0; q < divisions-1; q++){
      for(int w = 0; w < divisions-1; w++){
          int lesser = (VX[q] > VY[w]) ? VY[w] : VX[q];
          if(lesser % 2 != 0){
            (newInfo[currLoc2] == 0) ? newInfo[currLoc2] = 1 : newInfo[currLoc2] = 0;
          }
          currLoc2++;
      }
    }

    printf("\n");

    printf("Final version of data read: ");
    for (int g = 0; g < 16; ++g)
        printf("%d ", newInfo[g]);

  //test 2 - 18 bit write with 1 SA
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
