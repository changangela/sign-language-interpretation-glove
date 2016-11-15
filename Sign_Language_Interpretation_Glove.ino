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
//void FlexInit();

//void FlexTick();
void ShakeTick();
void BoardUITick();

// extern const uint32_t potentiometer;

void setup() 
{
  WireInit();
  Serial.begin(9600);

  delay(100);

  ShakeInit();
  BoardUIInit();
  FlexInit();

  // read some analog signal and scale it arbitrarily
  // for random seed.
  
  // srand((7+analogRead(potentiometer)) * 347);
}

void loop() 
{
  ShakeTick();
  BoardUITick();
  // FlexTick();
}
