#include <Wire.h>
#include <string.h>


void WireRequestArray(int address, uint8_t* buffer, uint8_t amount);
void WireWriteRegister(int address, uint8_t reg, uint8_t value);
void WireWriteByte(int address, uint8_t value);

// flex resistor pins
const uint32_t flexCount = 5;
const uint32_t mapLow = 0;
const uint32_t mapHigh = 10;

// the A0 pin is shared with the potentiometer, turn potentiometer fully clockwise for best flex detection
// these pins have been tested to work the best with the orbit booster pack attached
const int flex[flexCount] = {A1, A2, A3, A6, A11};

// mapping values of minimum and maximum flex voltages
int flexMin[flexCount] = {0};
int flexMax[flexCount] = {0};
int flexValue[flexCount]={0};

void boosterPackUndoRead(){
  // default pin configurations of the orbit booster pack
  // the ones that are digitalRead are not listed
  
  for(int i = 0; i < flexCount; ++i){
    analogWrite(flex[i], LOW);
  }
}

void resetFlex(){
  for(int i = 0; i < flexCount; ++i){
    flexMax[i] = 0;
    flexMin[i] = 0;
  }
}

void flexRead(){
  for(int i = 0; i < flexCount; ++i){
    flexValue[i] = map(analogRead(flex[i]), flexMin[i], flexMax[i], mapLow, mapHigh);
  }
  boosterPackUndoRead();
  
  // serial monitor print for debugging
  Serial.print(flexValue[0]);
  Serial.print("||");
  Serial.print(flexValue[1]);
  Serial.print("||");
  Serial.print(flexValue[2]);
  Serial.print("||");
  Serial.print(flexValue[3]);
  Serial.print("||");
  Serial.print(flexValue[4]);
  Serial.println("||");
  
  delay(500);
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
  boosterPackUndoRead();
  
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
  boosterPackUndoRead();
  
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
