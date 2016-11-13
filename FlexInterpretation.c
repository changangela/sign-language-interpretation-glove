#include "FlexInterpretation.h"

// due to the fact that variably sized arrays cannot be variably modified, 
// number of fingers was kept as the literal '5', as opposed to a constant

// the flex value at 180 degree acting as a buffer
 const unsigned int VALUE_OF_LINE = 180;

// configurations
 const unsigned int LETTERS_IN_ALPHABET = 26;

int a[][5] = {
  {0,0,0,0,0}, // 'a'
  {0,0,0,0,0}, // 'b'
  {0,0,0,0,0}, // 'c'
  {0,0,0,1,1}, // 'd'
  {0,0,0,0,0}, // 'e'
  {0,0,0,0,0}, // 'f'
  {0,0,0,0,0}, // 'g'
  {0,0,0,0,0}, // 'h'
  {0,0,0,0,0}, // 'i'
  {0,0,0,0,0}, // 'j'
  {0,0,0,0,0}, // 'k'
  {0,0,0,0,0}, // 'l'
  {0,0,0,0,0}, // 'm'
  {0,0,0,0,0}, // 'n'
  {0,0,0,0,0}, // 'o'
  {0,0,0,0,0}, // 'p'
  {0,0,0,0,0}, // 'q'
  {0,0,0,0,0}, // 'r'
  {0,0,0,0,0}, // 's'
  {0,0,0,0,0}, // 't'
  {0,0,0,0,0}, // 'u'
  {0,0,0,0,0}, // 'v'
  {0,0,0,0,0}, // 'w'
  {0,0,0,0,0}, // 'x'
  {0,0,0,0,0}, // 'y'
  {0,0,0,0,0} // 'z'
};


char translate(int f[]){
  char interpretation;
  int mindiff = -1;

  for(int i = 0; i < LETTERS_IN_ALPHABET; ++i){
    int epsilon = 0;
    for(int j = 0; j < 5; ++j){
      epsilon += (a[i][j] - f[j] >= 0) ? a[i][j] - f[j] : f[j] - a[i][j];
    }
    if(mindiff == -1 || mindiff > epsilon){
      mindiff = epsilon;
      interpretation  = 'a' + i;
    }
  }

  return interpretation;
}


