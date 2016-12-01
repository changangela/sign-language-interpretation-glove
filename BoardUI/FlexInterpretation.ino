// due to the fact that variably sized arrays cannot be variably modified, 
// number of fingers was kept as the literal '5', as opposed to a constant
// the flex value at 180 degree acting as a buffer
const unsigned int VALUE_OF_LINE = 180;
// configurations
const unsigned int LETTERS_IN_ALPHABET = 26;

//int a[][5] = {
//  {0,10,10,10,10}, // 'a'
//  {10,0,0,0,0}, // 'b'
//  {5,5,5,5,5}, // 'c' SIMILAR TO 'o' 
//  {10,0,10,10,10}, // 'd' SIMILAR TO 'z' | d=xmin*2 z=xmax 
//  {10,8,8,8,8}, // 'e' SIMILAR TO 'm', 'n', 's'
//  {5,7,0,0,0}, // 'f'
//  {0,0,10,10,10}, // 'g' SIMILAR TO 'q'  | g = xmax q = xmin 
//  {5,0,0,8,8}, // 'h' SIMILAR TO 'u' | h = xmax u = xmin*2
//  {10,10,10,10,0}, // 'i' SIMILAR TO 'j' | i = xmin*2 j = xmax
//  {10,10,10,10,3}, // 'j' SIMILAR TO 'i'
//  {0,0,5,10,10}, // 'k' SIMILAR TO p | k =  xmin*2 p = xmin
//  {0,0,9,9,9}, // 'l' SIMILAR TO 'g'
//  {10,8,6,6,6}, // 'm' SIMILAR TO 'e', 'n', 's'
//  {10,8,8,10,10}, // 'n' SIMILAR TO 'e', 'm', 's'
//  {8,7,7,7,7}, // 'o' SIMILAR TO 'o'
//  {0,0,3,10,10}, // 'p' SIMILAR to 
//  {0,3,10,10,10}, // 'q' SIMILAR TO 'k'
//  {8,1,3,7,9}, // 'r'
//  {10,10,10,10,10}, // 's' SIMILAR TO 'e', 'm', 'n'
//  {0,6,10,10,10}, // 't'
//  {7,0,0,7,7}, // 'u' SIMILAR TO 'v' | contact points open
//  {7,2,2,7,7}, // 'v' SIMILAR TO 'u'
//  {7,0,0,0,7}, // 'w'
//  {10,5,10,10,10}, // 'x'
//  {0,10,10,10,0}, // 'y'
//  {7,0,10,10,10} // 'z' SIMILAR TO 'd'
//};


/********************************************************************
 *                                                                  *
 *                           DATASET # 3                            *
 *                                                                  *
 ********************************************************************/
int a[][5] = {
  {30,99,98,97,95}, // 'a'
  {60,20,15,10,5}, // 'b'
  {48,60,60,67,60}, // 'c'
  {65,15,84,90,86}, // 'd'
  {83,95,95,98,90}, // 'e'
  {70,98,27,12,17}, // 'f'
  {35,35,90,93,90}, // 'g'
  {57,16,22,83,76}, // 'h'
  {70,100,99,88,13}, // 'i'
  {70,100,98,87,35}, // 'j'
  {15,30,40,75,60}, // 'k'
  {0,0,86,81,88}, // 'l'
  {76,90,87,87,85}, // 'm' ....
  {60,90,83,82,85}, // 'n'
  {70,80,80,71,64}, // 'o'
  {10,10,65,75,82}, // 'p'
  {14,30,82,78,82}, // 'q'
  {67,18,25,83,71}, // 'r'
  {72,100,96,93,93}, // 's'
  {61,100,96,93,93}, // 't'
  {70,5,6,77,68}, // 'u'
  {59,0,2,79,68}, // 'v'
  {68,25,20,16,76}, // 'w'
  {74,54,97,95,97}, // 'x'
  {2,76,82,63,4}, // 'y'
  {8,25,83,76,65} // 'z'
};

char translate(int f[], int16_t accel[]){
  // taking eeprom data, translation uses the K nearest neighbors method of training (regression)
   
  char interpretation;
  int mindiff = -1;
  for(int i = 0; i < LETTERS_IN_ALPHABET; ++i){
    int epsilon = 0;
    for(int j = 0; j < 5; ++j){
      //      epsilon += (a[i][j] - f[j] >= 0) ? a[i][j] - f[j] : f[j] - a[i][j];
      epsilon += (a[i][j] - f[j]) * (a[i][j] - f[j]);
    }
    if(mindiff == -1 || mindiff > epsilon){
      mindiff = epsilon;
      interpretation  = 'a' + i;
    }
  }

  // once the flex data is read and applied
  // the accelerometer data is used to distinguish between similar characters

  if(accel[0] < - mapHigh / 2){
    // hand accel is close to twice the calibration map in the x direction
    // theoretically (-mapHigh)

    switch(interpretation){
      case 'z':
        return 'd';
      case 'h':
        {
          int epsilonr = 0;
          int epsilonu = 0;
          int epsilonv = 0;
          for(int j = 0; j < 5; ++j){
            epsilonr += (a[17][j] - f[j]) * (a[17][j] - f[j]);
            epsilonu += (a[20][j] - f[j]) * (a[20][j] - f[j]);
            epsilonv += (a[21][j] - f[j]) * (a[21][j] - f[j]);
          }
          return 'a' + (epsilonr < epsilonu) ? (epsilonv < epsilonr) ? epsilonv : epsilonr : (epsilonv < epsilonu) ? epsilonv : epsilonu;
        }
      case 'j':
        return 'i';
      case 'p':
        return 'k';
    }    
  }

  return interpretation;
}

