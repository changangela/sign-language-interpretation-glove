#include <stdbool.h>
#include <string.h>

static enum SerialPages
{
  welcome = 0,
  minCalibration = 1,
  maxCalibration = 2,
  interpretation = 3,
} boardUIPage = welcome;

const uint32_t switchCount = 2;
const uint32_t buttonCount = 1;

const uint32_t buttons[buttonCount] = {PF_4};
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
  bool                switches[switchCount];
  struct ButtonState  buttons[buttonCount];
  float               potentiometer;
} boardInputState;

void SerialInit()
{
  
  
  boardInputState = { 0 };
  // activeBoard = { 0 };
  
  for(int i = 0; i < buttonCount; ++i )
    pinMode(buttons[i], INPUT_PULLUP);
}

/********************************************************************
 *                                                                  *
 *                         INTERPRETATION                           *
 *                                                                  *
 ********************************************************************/
 
static void handlePageInterpretation(){
  if(tickCounter == 1){
    Serial.println("Interpretation: ");
  }
  

  char interpretedLetter = translate(flexRead(), accelGyroRead());
  
  Serial.println(interpretedLetter);

  // FOLLOWING DELAY ONLY FOR DEBUGGING PURPOSES
  delay(100);
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
    Serial.println("Maximum calibration in:");
    Serial.println("3...");
    delay(1000);

    Serial.println("2...");
    delay(1000);

    Serial.println("1...");
    delay(1000);
  }    

  int numReadings = 100;
   
  // the flex sensor is read numerous times and the average value is taken
  if(tickCounter < numReadings){
    // error message if no data is read
    accelGyroMaxCalibration();
    if(!flexMaxCalibration()){
      Serial.println("Invalid reading...");
      resetFlex();
      resetGyroAccel();
      delay(1000);
    }    
  }else if(tickCounter == numReadings){
    // takes the average value of all readings
    accelGyroMaxAverage(numReadings);
    if(flexMaxAverage(numReadings)){
      Serial.println("Successful! Press button to continue...");
      while(boardUIPage == maxCalibration){
        for(int i = 0; i < buttonCount; ++i){
           if(!digitalRead(buttons[i])){
             while(!digitalRead(buttons[i])){
             }
             boardUIPage = interpretation;
             tickCounter = 0;
           }
        }
      }
    }else{
      
      // if the flexMax values were not greater than the flexMin values, there was an error
      // the calibration process must start over
      Serial.println("Unsuccessful, please try again");
      delay(1000);
    }
  }else{
    for(int i = 0; i < buttonCount; ++i){
      if(boardInputState.buttons[i].isRising){
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
    Serial.println("Minimum calibration in:");
    Serial.println("3...");
    delay(1000);

    Serial.println("2...");
    delay(1000);

    Serial.println("1...");
    delay(1000);
  }
     
     
  int numReadings = 100;
  
  // the flex sensor is read numerous times and the average value is taken
  if(tickCounter < numReadings){
    // reads data and error message if no data is read
    accelGyroMinCalibration();
    if(!flexMinCalibration()){
      resetFlex();
      resetGyroAccel();
      Serial.println("Invalid reading...");
      tickCounter = 0;
      delay(1000);
    }    
  }else if(tickCounter == numReadings){
    // takes the average value of all readings
    accelGyroMinAverage(numReadings);
    if(flexMinAverage(numReadings)){
      Serial.println("Successful! Press button to continue...");
      while(boardUIPage == minCalibration){
        for(int i = 0; i < buttonCount; ++i){
           if(!digitalRead(buttons[i])){
             while(!digitalRead(buttons[i])){
             }
             boardUIPage = maxCalibration;
             tickCounter = 0;
           }
        }
      }
    }else{
      // if there was an error, the calibration process must start over
      tickCounter = 0;
      resetFlex();
      Serial.println("Unsuccessful, please try again");
      delay(1000);
    }
  }else{
    for(int i = 0; i < buttonCount; ++i){
      if(boardInputState.buttons[i].isRising){
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

  while(!Serial){
  }
  
  Serial.println("Welcome to Sign Language Interpretation Glove");
  Serial.println("Press button to continue...");

  while(boardUIPage == welcome){
    for(int i = 0; i < buttonCount; ++i){
       if(!digitalRead(buttons[i])){
         while(!digitalRead(buttons[i])){
         }
         boardUIPage = minCalibration;
         tickCounter = 0;
       }
    }
  }
}

void SerialTick()
{
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

  tickCounter++;
}

