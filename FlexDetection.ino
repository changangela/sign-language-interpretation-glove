#include <Wire.h>
#include <string.h>
#include <stdio.h>


void WireRequestArray(int address, uint8_t* buffer, uint8_t amount);
void WireWriteRegister(int address, uint8_t reg, uint8_t value);
void WireWriteByte(int address, uint8_t value);

// flex resistor pins
const uint32_t flexCount = 5;
const uint32_t mapLow = 0;
extern const uint32_t mapHigh = 100;

// the A0 pin is shared with the potentiometer, turn potentiometer fully clockwise for best flex detection
// these pins have been tested to work the best with the orbit booster pack attached
const int flex[flexCount] = {A1,A2,A4,A5,A6};

// mapping values of minimum and maximum flex voltages
int flexMin[flexCount] = {0};
int flexMax[flexCount] = {0};
int flexLetter[26][flexCount] = {0};
int flexValue[flexCount]={0};

void resetFlex(){
  for(int i = 0; i < flexCount; ++i){
    flexMax[i] = 0;
    flexMin[i] = 0;
  }
}

void resetLetter(int letter){
  for(int i = 0; i < 26; ++i){
    flexLetter[letter][i] = 0;
  }
}

//void newLetter(){
//  for(int i = 0; i < 26; ++i){
//    for(int j = 0; j < flexCount; ++j){
//      flexLetter[i][j] = 0;
//    }
//  }
//}

int* flexRead(){
  // function returns a list of 5 flex values
  
  for(int i = 0; i < flexCount; ++i){
    flexValue[i] = map(analogRead(flex[i]), flexMin[i], flexMax[i], mapLow, mapHigh);
  }

  //  boosterPackUndoRead();
  
  // serial monitor print for debugging
  Serial.print(flexValue[0]);
  Serial.print(" || ");
  Serial.print(flexValue[1]);
  Serial.print(" || ");
  Serial.print(flexValue[2]);
  Serial.print(" || ");
  Serial.print(flexValue[3]);
  Serial.print(" || ");
  Serial.print(flexValue[4]);
  Serial.print(" || ");

  return flexValue;
  
}

int flexLetterCalibration(int letter){
  
  // calibrates the value of all 5 flex resistors when hand is at said value
  
  int tempLetter[flexCount];
  for(int i = 0; i < flexCount; ++i){
    tempLetter[i] = analogRead(flex[i]);
    if(!tempLetter[i]) return 0;
    flexLetter[letter][i] += tempLetter[i];
  }
  
  // since the same ports the flex sensors use are also mapped to booster pack components
  // when reading the values of the booster pack component, the read is 'undone'
  // boosterPackUndoRead();
  
  // serial monitor print for debugging
  
  Serial.print("Letter: ");
  Serial.print(flexLetter[letter][0]);
  Serial.print("||");
  Serial.print(flexLetter[letter][1]);
  Serial.print("||");
  Serial.print(flexLetter[letter][2]);
  Serial.print("||");
  Serial.print(flexLetter[letter][3]);
  Serial.print("||");
  Serial.print(flexLetter[letter][4]);
  Serial.println("||");
      
  return 1; 
}

int flexMaxCalibration(){
  
  // calibrates the value of all 5 flex resistors when hand is open
  
  int tempMax[flexCount];
  for(int i = 0; i < flexCount; ++i){
    tempMax[i] = analogRead(flex[i]);
    if(!tempMax[i]) return 0;
    flexMax[i] += tempMax[i];
  }
  
  // since the same ports the flex sensors use are also mapped to booster pack components
  // when reading the values of the booster pack component, the read is 'undone'
//  boosterPackUndoRead();
  
  // serial monitor print for debugging
  Serial.print("Max: ");
  Serial.print(flexMax[0]);
  Serial.print("||");
  Serial.print(flexMax[1]);
  Serial.print("||");
  Serial.print(flexMax[2]);
  Serial.print("||");
  Serial.print(flexMax[3]);
  Serial.print("||");
  Serial.print(flexMax[4]);
  Serial.println("||");
    
  return 1; 
}

int flexMinCalibration(){
  // calibrates the value of all 5 flex resistors when hand is closed
  
  int tempMin[flexCount];
  for(int i = 0; i < flexCount; ++i){
    tempMin[i] = analogRead(flex[i]);
    if(!tempMin[i]) return 0;
    flexMin[i] += tempMin[i];
  }
  
  // since the same ports the flex sensors use are also mapped to booster pack components
  // when reading the values of the booster pack component, the read is 'undone'
//  boosterPackUndoRead();
  
  // prints readings to serial monitor for debugging purposes
  Serial.print("Min: ");
  Serial.print(flexMin[0]);
  Serial.print("||");
  Serial.print(flexMin[1]);
  Serial.print("||");
  Serial.print(flexMin[2]);
  Serial.print("||");
  Serial.print(flexMin[3]);
  Serial.print("||");
  Serial.print(flexMin[4]);
  Serial.println("||");

  return 1; 
}

int flexLetterAverage(int letter, int numReadings){
  // divides the flexLetter readings by the number of readings to take the average reading
  // for a more accurate calibration
  
  for(int i = 0; i < flexCount; ++i){
    flexLetter[letter][i] /= numReadings;
  }
  
  // serial print for debugging purpurses
  Serial.print("Average: ");
  Serial.print(flexLetter[letter][0]);
  Serial.print("||");
  Serial.print(flexLetter[letter][1]);
  Serial.print("||");
  Serial.print(flexLetter[letter][2]);
  Serial.print("||");
  Serial.print(flexLetter[letter][3]);
  Serial.print("||");
  Serial.print(flexLetter[letter][4]);
  Serial.println("||");
  
  Serial.print("Letter: ");
  Serial.print(map(flexLetter[letter][0], flexMin[0], flexMax[0], mapLow, mapHigh));
  Serial.print("||");
  Serial.print(map(flexLetter[letter][1], flexMin[1], flexMax[1], mapLow, mapHigh));
  Serial.print("||");
  Serial.print(map(flexLetter[letter][2], flexMin[2], flexMax[2], mapLow, mapHigh));
  Serial.print("||");
  Serial.print(map(flexLetter[letter][3], flexMin[3], flexMax[3], mapLow, mapHigh));
  Serial.print("||");
  Serial.print(map(flexLetter[letter][4], flexMin[4], flexMax[4], mapLow, mapHigh));
  Serial.println("||");
  
  // write calibrated data into text file to be fed to neural network
  
  for(int i = 0; i <= letter; ++i){
    Serial.print("{");
    Serial.print(map(flexLetter[i][0], flexMin[0], flexMax[0], mapLow, mapHigh));
    Serial.print(",");
    Serial.print(map(flexLetter[i][1], flexMin[1], flexMax[1], mapLow, mapHigh));
    Serial.print(",");
    Serial.print(map(flexLetter[i][2], flexMin[2], flexMax[2], mapLow, mapHigh));
    Serial.print(",");
    Serial.print(map(flexLetter[i][3], flexMin[3], flexMax[3], mapLow, mapHigh));
    Serial.print(",");
    Serial.print(map(flexLetter[i][4], flexMin[4], flexMax[4], mapLow, mapHigh));
    Serial.println("}");
  }
  
  return 1;
  
}

int flexMinAverage(int numReadings){
  // divides the flexMin readings by the number of readings to take the average reading
  // for a more accurate calibration
  
  for(int i = 0; i < flexCount; ++i){
    flexMin[i] /= numReadings;
  }
  
  // serial print for debugging purpurses
  Serial.print("Average: ");
  Serial.print(flexMin[0]);
  Serial.print("||");
  Serial.print(flexMin[1]);
  Serial.print("||");
  Serial.print(flexMin[2]);
  Serial.print("||");
  Serial.print(flexMin[3]);
  Serial.print("||");
  Serial.print(flexMin[4]);
  Serial.println("||");
  
  return 1;
  
}

int flexMaxAverage(int numReadings){
  // divides the flexMax readings by the number of readings to take the average reading
  // for a more accurate calibration
  
  for(int i = 0; i < flexCount; ++i){
    flexMax[i] /= numReadings;
//    if(!(flexMax[i] > flexMin[i])){
//      // the flexMax value must be greater than FlexMin value
//      return 0;
//    }
  }
  
  // serial print for debugging purpurses
  Serial.print("Average: ");
  Serial.print(flexMax[0]);
  Serial.print("||");
  Serial.print(flexMax[1]);
  Serial.print("||");
  Serial.print(flexMax[2]);
  Serial.print("||");
  Serial.print(flexMax[3]);
  Serial.print("||");
  Serial.print(flexMax[4]);
  Serial.println("||");
  
  return 1;
}

void FlexInit()
{
  Serial.begin(9600);
  // SerialDebugInit();
}


void FlexTick()
{
  // flexRead();
}

void SerialDebugInit(){
  int countdown = 3;
  int numReadings = 100;
  
  Serial.println("Minimum calibration in: ");
  for(int i = countdown; i > 0; --i){
    Serial.println( i );
    delay(1000);
  }  
  
  for(int i = 0; i < numReadings; ++i){
    flexMinCalibration();
  }
  flexMinAverage(numReadings);
  
  Serial.println("Maximum calibration in: ");
  for(int i = countdown; i > 0; --i){
    Serial.println( i );
    delay(1000);
  }

  for(int i = 0; i < numReadings; ++i){
    flexMaxCalibration();
  }
  flexMaxAverage(numReadings);
    
}
