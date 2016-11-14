#include <stdbool.h>
#include <string.h>

static enum BoardPages
{
  welcome = 0,
  calibration = 2,
} boardUIPage = welcome;

const uint32_t switchCount = 2;
const uint32_t buttonCount = 2;
const uint32_t switches[switchCount] = { PA_7, PA_6 };
const uint32_t buttons[buttonCount] = { PD_2, PE_0 };
const uint32_t potentiometer = PE_3;

int rowHeight = 12;
int tickCounter = 0;

struct ButtonState
{ 
  bool state;
  bool isRising;
};

struct BoardState
{
} activeBoard;

static struct InputState
{
  bool                switches[2];
  struct ButtonState  buttons[2];
  float               potentiometer;
} boardInputState;

void BoardUIInit()
{
  OrbitOledInit();
  OrbitOledClear();
  OrbitOledClearBuffer();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);

  boardInputState = { 0 };
  // activeBoard = { 0 };

  for(int i = 0; i < switchCount; ++i )
    pinMode(switches[i], INPUT);
  for(int i = 0; i < buttonCount; ++i )
    pinMode(buttons[i], INPUT);
}

static void handlePageCalibration()
{
  OrbitOledClear();
  OrbitOledMoveTo(0, rowHeight * 0);
  OrbitOledDrawString("Calibration for ");
  OrbitOledMoveTo(0, rowHeight * 1);
  OrbitOledDrawString("minimum in:");
  OrbitOledMoveTo(0, rowHeight * 2);
  OrbitOledDrawString("3...");
  OrbitOledUpdate(); 
  delay(1000);
  
  OrbitOledClear();
  OrbitOledMoveTo(0, rowHeight * 0);
  OrbitOledDrawString("Calibration for ");
  OrbitOledMoveTo(0, rowHeight * 1);
  OrbitOledDrawString("minimum in:");
  OrbitOledMoveTo(0, rowHeight * 2);
  OrbitOledDrawString("2...");
  OrbitOledUpdate(); 
  delay(1000);
  
  OrbitOledClear();
  OrbitOledMoveTo(0, rowHeight * 0);
  OrbitOledDrawString("Calibration for ");
  OrbitOledMoveTo(0, rowHeight * 1);
  OrbitOledDrawString("minimum in:");
  OrbitOledMoveTo(0, rowHeight * 2);
  OrbitOledDrawString("1...");
  OrbitOledUpdate(); 
  delay(1000);
}

static void handlePageWelcome(){
  // displays "Sign language interpretation glove" while checking for button press
  int flashRate = 400;
  if((tickCounter / flashRate) % 2 == 0 && 
     (tickCounter - (tickCounter / flashRate) * flashRate) == 0)
  {
    OrbitOledClear();
    OrbitOledMoveTo(0,rowHeight * 0);
    OrbitOledDrawString("Sign Language");
    OrbitOledMoveTo(0,rowHeight * 1);
    OrbitOledDrawString("Interpretation");
    OrbitOledMoveTo(0,rowHeight * 2);
    OrbitOledDrawString("Glove");
  }
  
  if((tickCounter / flashRate) % 2 == 1 && 
     (tickCounter - (tickCounter / flashRate) * flashRate) == 0){ 
    OrbitOledClear();
    OrbitOledMoveTo(0,rowHeight * 0);
    OrbitOledDrawString("Press button");
    OrbitOledMoveTo(0, rowHeight * 1);
    OrbitOledDrawString("to continue...");
  }
  
  for(int i = 0; i < buttonCount; ++i){
    if(boardInputState.buttons[i].isRising){
      OrbitOledClearBuffer();
      OrbitOledClear();
      boardUIPage = calibration;
    }
  }

}

static void UIInputTick()
{
  for(int i = 0; i < switchCount; ++i )
    boardInputState.switches[i] = digitalRead(switches[i]);
  for(int i = 0; i < buttonCount; ++i )
  {
    // Only look for Rising Edge Signals.
    bool previousState = boardInputState.buttons[i].state;
    boardInputState.buttons[i].state = digitalRead(buttons[i]);
    boardInputState.buttons[i].isRising = (!previousState && boardInputState.buttons[i].state);
  }
  boardInputState.potentiometer = analogRead(potentiometer);
}

void BoardUITick()
{
  UIInputTick();
  switch(boardUIPage){
  case welcome:
    handlePageWelcome();
    break;
  case calibration:
    handlePageCalibration();
    break;
  }

  OrbitOledUpdate();
  tickCounter++;
}

