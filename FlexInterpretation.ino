// due to the fact that variably sized arrays cannot be variably modified, 
// number of fingers was kept as the literal '5', as opposed to a constant
// the flex value at 180 degree acting as a buffer
 const unsigned int VALUE_OF_LINE = 180;
// configurations
 const unsigned int LETTERS_IN_ALPHABET = 26;
 
int a[][5] = {
  {0,10,10,10,10}, // 'a'
  {10,0,0,0,0}, // 'b'
  {5,5,5,5,5}, // 'c' SIMILAR TO 'o'
  {10,0,10,10,10}, // 'd' SIMILAR TO 'z'
  {10,8,8,8,8}, // 'e' SIMILAR TO 'm', 'n', 's'
  {5,7,0,0,0}, // 'f'
  {0,0,10,10,10}, // 'g' SIMILAR TO 'l'
  {5,0,0,8,8}, // 'h' SIMILAR TO 'u'
  {10,10,10,10,0}, // 'i' SIMILAR TO 'j'
  {10,10,10,10,3}, // 'j' SIMILAR TO 'i'
  {0,0,5,10,10}, // 'k' SIMILAR TO q
  {0,0,9,9,9}, // 'l' SIMILAR TO 'g'
  {10,8,6,6,6}, // 'm' SIMILAR TO 'e', 'n', 's'
  {10,8,8,10,10}, // 'n' SIMILAR TO 'e', 'm', 's'
  {8,7,7,7,7}, // 'o' SIMILAR TO 'o'
  {0,0,3,10,10}, // 'p' SIMILAR to 
  {0,3,10,10,10}, // 'q' SIMILAR TO 'k'
  {8,1,3,7,9}, // 'r'
  {10,10,10,10,10}, // 's' SIMILAR TO 'e', 'm', 'n'
  {0,6,10,10,10}, // 't'
  {7,0,0,7,7}, // 'u' SIMILAR TO 'v'
  {7,2,2,7,7}, // 'v' SIMILAR TO 'u'
  {7,0,0,0,7}, // 'w'
  {10,5,10,10,10}, // 'x'
  {0,10,10,10,0}, // 'y'
  {7,0,10,10,10} // 'z' SIMILAR TO 'd'
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
