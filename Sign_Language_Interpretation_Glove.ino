#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <stdlib.h>

void WireInit();
void BoardInit();
void ShakeInit();
void SerialInit();

//void FlexInit();

//void FlexTick();
void ShakeTick();
void BoardUITick();

// extern const uint32_t potentiometer;

void setup() 
{
  // WireInit();

  delay(100);
  Serial.begin(9600);

  // ShakeInit();
  SerialInit();
  FlexInit();

  // read some analog signal and scale it arbitrarily
  // for random seed.
  
  // srand((7+analogRead(potentiometer)) * 347);
}

void loop() 
{
  // ShakeTick();
  // BoardUITick();
  SerialTick();
  // FlexTick();
}
