#include <stdbool.h>
#include <string.h>

const uint32_t SwitchCount = 2;
const uint32_t ButtonCount = 2;
const uint32_t Switches[SwitchCount] = { PA_7, PA_6 };
const uint32_t Buttons[ButtonCount] = { PD_2, PE_1 };
const uint32_t Potentiometer = PE_3;


void BoardUIInit()
{
  OrbitOledInit();
  OrbitOledClear();
  OrbitOledClearBuffer();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);

  for(int i = 0; i < SwitchCount; ++i )
    pinMode(Switches[i], INPUT);
  for(int i = 0; i < ButtonCount; ++i )
    pinMode(Buttons[i], INPUT);
}

void BoardUITick()
{
  /*OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("Sign Language");
  OrbitOledMoveTo(0, 12);
  OrbitOledDrawString("Interpretation");
  OrbitOledUpdate();*/
}
