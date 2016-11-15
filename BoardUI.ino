#include <stdbool.h>
#include <string.h>

static enum BoardPages
{
  welcome = 0,
  minCalibration = 1,
  maxCalibration = 2,
  interpretation = 3,
} boardUIPage = welcome;

const uint32_t switchCount = 2;
const uint32_t buttonCount = 2;


const uint32_t switches[switchCount] = { PA_7, PA_6 };
const uint32_t buttons[buttonCount] = { PD_2, PE_0 };
// const uint32_t potentiometer = PE_3;

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
  int                 flex[2];
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

/********************************************************************
 *                                                                  *
 *                         INTERPRETATION                           *
 *                                                                  *
 ********************************************************************/
 
static void handlePageInterpretation(){
  if(tickCounter == 1){
    OrbitOledClear();
    OrbitOledMoveTo(0, rowHeight * 0);
    OrbitOledDrawString("Interpretation: ");
    OrbitOledUpdate(); 
  }
  
  flexRead();
}

/********************************************************************
 *                                                                  *
 *                      CALIBRATION: MAX PAGE                       *
 *                                                                  *
 ********************************************************************/
 
static void handlePageMaxCalibration()
{
  // print out user instructions on the first iteration
  if(tickCounter == 1){
    OrbitOledClear();
    OrbitOledMoveTo(0, rowHeight * 0);
    OrbitOledDrawString("Calibration for ");
    OrbitOledMoveTo(0, rowHeight * 1);
    OrbitOledDrawString("maximum in:");
    OrbitOledMoveTo(0, rowHeight * 2);
    OrbitOledDrawString("3...");
    OrbitOledUpdate(); 
    delay(1000);
    
    OrbitOledClear();
    OrbitOledMoveTo(0, rowHeight * 0);
    OrbitOledDrawString("Calibration for ");
    OrbitOledMoveTo(0, rowHeight * 1);
    OrbitOledDrawString("maximum in:");
    OrbitOledMoveTo(0, rowHeight * 2);
    OrbitOledDrawString("2...");
    OrbitOledUpdate(); 
    delay(1000);
    
    OrbitOledClear();
    OrbitOledMoveTo(0, rowHeight * 0);
    OrbitOledDrawString("Calibration for ");
    OrbitOledMoveTo(0, rowHeight * 1);
    OrbitOledDrawString("maximum in:");
    OrbitOledMoveTo(0, rowHeight * 2);
    OrbitOledDrawString("1...");
    OrbitOledUpdate(); 
    delay(1000);
    
    // the screen will display validating for the duration of minimum calibration
    OrbitOledClear();
    OrbitOledMoveTo(0, rowHeight * 0);
    OrbitOledDrawString("Validating...");
    OrbitOledMoveTo(0, rowHeight * 1);
    OrbitOledDrawString("PLEASE DO NOT");
    OrbitOledMoveTo(0, rowHeight * 2);
    OrbitOledDrawString("MOVE");
    OrbitOledUpdate();
  }    

  int numReadings = 100;
   
  // the flex sensor is read numerous times and the average value is taken
  if(tickCounter < numReadings){
    // error message if no data is read
    if(!flexMaxCalibration()){
      OrbitOledClear();
      OrbitOledMoveTo(0, rowHeight * 0);
      OrbitOledDrawString("Invalid reading...");
      OrbitOledUpdate();
      tickCounter = 0;
      resetFlex();
      delay(1000);
    }    
  }else if(tickCounter == numReadings){
    // takes the average value of all readings
    
    if(flexMaxAverage(numReadings)){
      OrbitOledClear();
      OrbitOledMoveTo(0,rowHeight * 0);
      OrbitOledDrawString("Successful!");
      OrbitOledMoveTo(0,rowHeight * 1);
      OrbitOledDrawString("Press button");
      OrbitOledMoveTo(0, rowHeight * 2);
      OrbitOledDrawString("to continue...");
    }else{
      
      // if the flexMax values were not greater than the flexMin values, there was an error
      // the calibration process must start over
      tickCounter = 0;
      boardUIPage = minCalibration;
      resetFlex();
      
      OrbitOledClear();
      OrbitOledMoveTo(0,rowHeight * 0);
      OrbitOledDrawString("Unsuccessful!");
      OrbitOledMoveTo(0,rowHeight * 1);
      OrbitOledDrawString("Please try again...");
      OrbitOledUpdate();
      delay(1000);
    }
  }else{
    for(int i = 0; i < buttonCount; ++i){
      if(boardInputState.buttons[i].isRising){
        OrbitOledClearBuffer();
        OrbitOledClear();
        boardUIPage = interpretation;
        tickCounter = 0;
      }
    }
  }
}

/********************************************************************
 *                                                                  *
 *                      CALIBRATION: MIN PAGE                       *
 *                                                                  *
 ********************************************************************/
 
static void handlePageMinCalibration()
{
  // first time print out user instructions
  if(tickCounter == 1){
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
    
    // the screen will display validating for the duration of minimum calibration
    OrbitOledClear();
    OrbitOledMoveTo(0, rowHeight * 0);
    OrbitOledDrawString("Validating...");
    OrbitOledMoveTo(0, rowHeight * 1);
    OrbitOledDrawString("PLEASE DO NOT");
    OrbitOledMoveTo(0, rowHeight * 2);
    OrbitOledDrawString("MOVE");
    OrbitOledUpdate();
  }
     
     
  int numReadings = 100;
  
  // the flex sensor is read numerous times and the average value is taken
  if(tickCounter < numReadings){
    // reads data and error message if no data is read
    if(!flexMinCalibration()){
      resetFlex();
      OrbitOledClear();
      OrbitOledMoveTo(0, rowHeight * 0);
      OrbitOledDrawString("Invalid reading...");
      OrbitOledUpdate();
      tickCounter = 0;
      delay(1000);
    }    
  }else if(tickCounter == numReadings){
    // takes the average value of all readings
    if(flexMinAverage(numReadings)){
      OrbitOledClear();
      OrbitOledMoveTo(0,rowHeight * 0);
      OrbitOledDrawString("Successful!");
      OrbitOledMoveTo(0,rowHeight * 1);
      OrbitOledDrawString("Press button");
      OrbitOledMoveTo(0, rowHeight * 2);
      OrbitOledDrawString("to continue...");
    }else{
      // if there was an error, the calibration process must start over
      tickCounter = 0;
      resetFlex();
      
      OrbitOledClear();
      OrbitOledMoveTo(0,rowHeight * 0);
      OrbitOledDrawString("Unsuccessful!");
      OrbitOledMoveTo(0,rowHeight * 1);
      OrbitOledDrawString("Please try");
      OrbitOledMoveTo(0,rowHeight * 2);
      OrbitOledDrawString("again...");
      OrbitOledUpdate();
      delay(1000);
    }
  }else{
    for(int i = 0; i < buttonCount; ++i){
      if(boardInputState.buttons[i].isRising){
        OrbitOledClearBuffer();
        OrbitOledClear();
        boardUIPage = maxCalibration;
        tickCounter = 0;
      }
    }
  }   
}

/********************************************************************
 *                                                                  *
 *                           WELCOME PAGE                           *
 *                                                                  *
 ********************************************************************/

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
      boardUIPage = minCalibration;
      tickCounter = 0;
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
  // boardInputState.potentiometer = analogRead(potentiometer);
}

void BoardUITick()
{
  UIInputTick();
  switch(boardUIPage){
  case welcome:
    handlePageWelcome();
    break;
  case minCalibration:
    handlePageMinCalibration();
    break;
  case maxCalibration:
    handlePageMaxCalibration();
    break;
  case interpretation:
    handlePageInterpretation();
    break;
  }

  OrbitOledUpdate();
  tickCounter++;
}

