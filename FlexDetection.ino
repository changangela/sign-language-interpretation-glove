
#include <Wire.h>
#include <string.h>

const int flex1 = A1;
const int flex2 = A2;
const int flex3 = A4;
const int flex4 = A5;
const int flex5 = A6;

int flex1min = 0;
int flex2min = 0;
int flex3min = 0;
int flex4min = 0;
int flex5min = 0;

int flex1max = 0;
int flex2max = 0;
int flex3max = 0;
int flex4max = 0;
int flex5max = 0;

void WireRequestArray(int address, uint8_t* buffer, uint8_t amount);
void WireWriteRegister(int address, uint8_t reg, uint8_t value);
void WireWriteByte(int address, uint8_t value);

void FlexInit()
{
  Serial.begin(9600);
}

void FlexTick()
{  
  int flex1Reading = analogRead(flex1);
  int flex2Reading = analogRead(flex2);
  int flex3Reading = analogRead(flex3);
  int flex4Reading = analogRead(flex4);
  int flex5Reading = analogRead(flex5);

  while(flex1min + flex2min + flex3min + flex4min + flex5min == 0){
    delay(5000);
    Serial.println("Please move flex sensors to the minimum position. Reading will be taken in: ");
    Serial.println("3...");
    delay(1000);
    Serial.println("2...");
    delay(1000);
    Serial.println("1...");
    delay(1000);
    int tempReading1 = analogRead(flex1);
    int tempReading2 = analogRead(flex2);
    int tempReading3 = analogRead(flex3);
    int tempReading4 = analogRead(flex4);
    int tempReading5 = analogRead(flex5);
    flex1min = tempReading1;
    flex2min = tempReading2;
    flex3min = tempReading3;
    flex4min = tempReading4;
    flex5min = tempReading5;
    if(flex1min + flex2min + flex3min + flex4min + flex5min == 0) Serial.println("Invalid reading. Please try again.");
    else Serial.println("Valid reading taken.");
  }

  while(flex1max + flex2max + flex3max + flex4max + flex5max == 0){
    delay(5000);
    Serial.println("Please move flex sensors to the maximum position. Reading will be taken in: ");
    Serial.println("3...");
    delay(1000);
    Serial.println("2...");
    delay(1000);
    Serial.println("1...");
    delay(1000);
    int tempReading1 = analogRead(flex1);
    int tempReading2 = analogRead(flex2);
    int tempReading3 = analogRead(flex3);
    int tempReading4 = analogRead(flex4);
    int tempReading5 = analogRead(flex5);
    flex1max = tempReading1;
    flex2max = tempReading2;
    flex3max = tempReading3;
    flex4max = tempReading4;
    flex5max = tempReading5;
    if(flex1max + flex2max + flex3max + flex4max + flex5max == 0) Serial.println("Invalid reading. Please try again.");
    else Serial.println("Valid reading taken.");
  }

  int flex1Mapped = map(flex1Reading, flex1min, flex1max, 0, 100) < 0 ? 0 : map(flex1Reading, flex1min, flex1max, 0, 100);
  int flex2Mapped = map(flex2Reading, flex2min, flex2max, 0, 100) < 0 ? 0 : map(flex2Reading, flex2min, flex2max, 0, 100);
  int flex3Mapped = map(flex3Reading, flex3min, flex3max, 0, 100) < 0 ? 0 : map(flex3Reading, flex3min, flex3max, 0, 100);
  int flex4Mapped = map(flex4Reading, flex4min, flex4max, 0, 100) < 0 ? 0 : map(flex4Reading, flex4min, flex4max, 0, 100);
  int flex5Mapped = map(flex5Reading, flex5min, flex5max, 0, 100) < 0 ? 0 : map(flex5Reading, flex5min, flex5max, 0, 100);
  
  //Serial.print(flex1Reading);
  //Serial.print(" / ");
  Serial.print(flex1Mapped);
  Serial.print(" || ");
  //Serial.print(flex2Reading);
  //Serial.print(" / ");
  Serial.print(flex2Mapped);
  //Serial.println("");
  Serial.print(" || ");
  //Serial.print(flex3Reading);
  //Serial.print(" / ");
  Serial.print(flex3Mapped);
  Serial.print(" || ");
  //Serial.print(flex4Reading);
  //Serial.print(" / ");
  Serial.print(flex4Mapped);
  Serial.print(" || ");
  //Serial.print(flex5Reading);
  //Serial.print(" / ");
  Serial.println(flex5Mapped);

  delay(250); //just here to slow down the output for easier reading
  /*OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("Sign Language");
  OrbitOledMoveTo(0, 12);
  OrbitOledDrawString("Interpretation");
  OrbitOledMoveTo(0, 24);
  OrbitOledDrawString("Glove");
  OrbitOledUpdate();

  while(flex1min + flex2min + flex3min + flex4min + flex5min <= 0){
    delay(5000);
    Serial.println("Please move flex sensors to the minimum position. Reading will be taken in: ");
    Serial.println("3...");
    OrbitOledClear();
    OrbitOledMoveTo(0, 0);
    OrbitOledDrawString("Calibration for");
    OrbitOledMoveTo(0, 12);
    OrbitOledDrawString("minimum in:");
    OrbitOledMoveTo(0, 24);
    OrbitOledDrawString("3...");
    OrbitOledUpdate();
    delay(1000);
    Serial.println("2...");
    OrbitOledClear();
    OrbitOledMoveTo(0, 0);
    OrbitOledDrawString("Calibration for");
    OrbitOledMoveTo(0, 12);
    OrbitOledDrawString("minimum in:");
    OrbitOledMoveTo(0, 24);
    OrbitOledDrawString("2...");
    OrbitOledUpdate();
    delay(1000);
    Serial.println("1...");
    OrbitOledClear();
    OrbitOledMoveTo(0, 0);
    OrbitOledDrawString("Calibration for");
    OrbitOledMoveTo(0, 12);
    OrbitOledDrawString("minimum in:");
    OrbitOledMoveTo(0, 24);
    OrbitOledDrawString("1...");
    OrbitOledUpdate();
    delay(1000);
    int tempReading1 = analogRead(flex1);
    int tempReading2 = analogRead(flex2);
    int tempReading3 = analogRead(flex3);
    int tempReading4 = analogRead(flex4);
    int tempReading5 = analogRead(flex5);
    flex1min = tempReading1;
    flex2min = tempReading2;
    flex3min = tempReading3;
    flex4min = tempReading4;
    flex5min = tempReading5;
    if(flex1min + flex2min + flex3min + flex4min + flex5min <= 0){
      Serial.println("Invalid reading. Please try again.");
      OrbitOledClear();
      OrbitOledMoveTo(0, 0);
      OrbitOledDrawString("Invalid Reading.");
      OrbitOledMoveTo(0, 12);
      OrbitOledDrawString("Please try again.");
      OrbitOledUpdate();
    }else{
      Serial.println("Valid reading taken.");
      OrbitOledClear();
      OrbitOledMoveTo(0, 12);
      OrbitOledDrawString("Valid reading taken.");
      OrbitOledUpdate();
    }
  }

  while(flex1max + flex2max + flex3max + flex4max + flex5max == 0 || flex1max <= flex1min && flex2max <= flex2min && flex3max <= flex3min && flex4max <= flex4min && flex5max <= flex5min){
    delay(5000);
    Serial.println("Please move flex sensors to the maximum position. Reading will be taken in: ");
    Serial.println("3...");
    delay(1000);
    Serial.println("2...");
    delay(1000);
    Serial.println("1...");
    delay(1000);
    int tempReading1 = analogRead(flex1);
    int tempReading2 = analogRead(flex2);
    int tempReading3 = analogRead(flex3);
    int tempReading4 = analogRead(flex4);
    int tempReading5 = analogRead(flex5);
    flex1max = tempReading1;
    flex2max = tempReading2;
    flex3max = tempReading3;
    flex4max = tempReading4;
    flex5max = tempReading5;
    if(flex1max <= flex1min && flex2max <= flex2min && flex3max <= flex3min && flex4max <= flex4min && flex5max <= flex5min){
      Serial.println("Invalid reading. Please try again.");
    }else{
      Serial.println("Valid reading taken.");
    }
  }

  int flex1Reading = analogRead(flex1);
  int flex2Reading = analogRead(flex2);
  int flex3Reading = analogRead(flex3);
  int flex4Reading = analogRead(flex4);
  int flex5Reading = analogRead(flex5);
  
  int flex1Mapped = map(flex1Reading, flex1min, flex1max, 0, 100) < 0 ? 0 : map(flex1Reading, flex1min, flex1max, 0, 100);
  int flex2Mapped = map(flex2Reading, flex2min, flex2max, 0, 100) < 0 ? 0 : map(flex2Reading, flex2min, flex2max, 0, 100);
  int flex3Mapped = map(flex3Reading, flex3min, flex3max, 0, 100) < 0 ? 0 : map(flex3Reading, flex3min, flex3max, 0, 100);
  int flex4Mapped = map(flex4Reading, flex4min, flex4max, 0, 100) < 0 ? 0 : map(flex4Reading, flex4min, flex4max, 0, 100);
  int flex5Mapped = map(flex5Reading, flex5min, flex5max, 0, 100) < 0 ? 0 : map(flex5Reading, flex5min, flex5max, 0, 100);
  
  //Serial.print(flex1Reading);
  //Serial.print(" / ");
  Serial.print(flex1Mapped);
  Serial.print(" || ");
  //Serial.print(flex2Reading);
  //Serial.print(" / ");
  Serial.print(flex2Mapped);
  //Serial.println("");
  Serial.print(" || ");
  //Serial.print(flex3Reading);
  //Serial.print(" / ");
  Serial.print(flex3Mapped);
  Serial.print(" || ");
  //Serial.print(flex4Reading);
  //Serial.print(" / ");
  Serial.print(flex4Mapped);
  Serial.print(" || ");
  //Serial.print(flex5Reading);
  //Serial.print(" / ");
  Serial.println(flex5Mapped);

  delay(500); //just here to slow down the output for easier reading*/
}

