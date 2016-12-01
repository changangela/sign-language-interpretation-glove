#include <stdbool.h>
#include <string.h>
#include <LiquidCrystal.h>

static enum SerialPages{
  welcome = 0,
  handOrLetter = 1,
  minCalibration = 2,
  maxCalibration = 3,
  letterCalibration = 4,
  interpretation = 5,
}boardUIPage = welcome;

const int button1 = 31;
const int button2 = 17;
const int pageDelay = 2000;

int button1State = 0;
int button2State = 0;
int fullHand = 0;

String alph = "abcdefghijklmnopqrstuvwxyz";
String result = "abcdefghijklmnopq";

/* DEL 
const uint32_t switchCount = 2;
const uint32_t buttonCount = 1;

const uint32_t buttons[buttonCount] = {PF_4};
// const uint32_t potentiometer = PE_3;

int rowHeight = 12;
int tickCounter = 0;
*/

LiquidCrystal lcd(2, 19, 37, 38, 39, 40);

/* DEL
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
} boardInputState;*/

//   #### ##    ## #### ########
//    ##  ###   ##  ##     ##
//    ##  ####  ##  ##     ##
//    ##  ## ## ##  ##     ##
//    ##  ##  ####  ##     ##
//    ##  ##   ###  ##     ##
//   #### ##    ## ####    ##
//
// INITIALIZATION
void SerialInit(){
  /* DEL
  boardInputState = { 0 };
  // activeBoard = { 0 };
  
  for(int i = 0; i < buttonCount; ++i )
    pinMode(buttons[i], INPUT_PULLUP);*/
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  lcd.begin(16, 2);
}

//   ########          ##      ## ######## ##        ######   #######  ##     ## ########
//   ##     ##         ##  ##  ## ##       ##       ##    ## ##     ## ###   ### ##
//   ##     ##         ##  ##  ## ##       ##       ##       ##     ## #### #### ##
//   ########          ##  ##  ## ######   ##       ##       ##     ## ## ### ## ######
//   ##                ##  ##  ## ##       ##       ##       ##     ## ##     ## ##
//   ##                ##  ##  ## ##       ##       ##    ## ##     ## ##     ## ##
//   ##        #######  ###  ###  ######## ########  ######   #######  ##     ## ########
//
// WELCOME PAGE
static void handlePageWel(){
  while(boardUIPage == welcome){
    delay(pageDelay);
    lcd.clear();
    lcd.print(" Welcome to the");
    lcd.setCursor(0, 1);
    lcd.print(" Sign language");
    delay(pageDelay);
    lcd.clear();
    lcd.print(" intepretation");
    lcd.setCursor(0, 1);
    lcd.print("     glove");
    delay(pageDelay);
    lcd.clear();
    lcd.print("Press BUTTON ONE");
    lcd.setCursor(0, 1);
    lcd.print(" to continue...");
    
    while(digitalRead(button1) != 0){}
    
    boardUIPage = handOrLetter;
  }
}

// DEPRECATED
/*static void handlePageWelcome(){
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
}*/

//   ########          ##     ##    ###    ##    ## ########        ## ##       ######## ########
//   ##     ##         ##     ##   ## ##   ###   ## ##     ##      ##  ##       ##          ##
//   ##     ##         ##     ##  ##   ##  ####  ## ##     ##     ##   ##       ##          ##
//   ########          ######### ##     ## ## ## ## ##     ##    ##    ##       ######      ##
//   ##                ##     ## ######### ##  #### ##     ##   ##     ##       ##          ##
//   ##                ##     ## ##     ## ##   ### ##     ##  ##      ##       ##          ##
//   ##        ####### ##     ## ##     ## ##    ## ########  ##       ######## ########    ##
//
// HAND OR LETTER PAGE

static void handlePageOrr(){
  while(boardUIPage == handOrLetter){
    lcd.clear();
    lcd.print("Press BUTTON ONE");
    lcd.setCursor(0, 1);
    lcd.print("to set full hand");
    delay(pageDelay);
    lcd.clear();
    lcd.print("Press BUTTON TWO");
    lcd.setCursor(0, 1);
    lcd.print("to set flex vals");
    delay(pageDelay);
    lcd.clear();
    lcd.print("full hand   / B2");
    lcd.setCursor(0, 1);
    lcd.print("B1 /   flex only");
    
    do{
      button1State = !digitalRead(button1);
      button2State = !digitalRead(button2);
    }while(button1State == 0 && button2State == 0);
    
    if(button1State != 0){
      fullHand = 1;
    }else{
      fullHand = 0;
    }
    boardUIPage = minCalibration;
  }
}

//   ########          ##     ## #### ##    ##  ######     ###    ##
//   ##     ##         ###   ###  ##  ###   ## ##    ##   ## ##   ##
//   ##     ##         #### ####  ##  ####  ## ##        ##   ##  ##
//   ########          ## ### ##  ##  ## ## ## ##       ##     ## ##
//   ##                ##     ##  ##  ##  #### ##       ######### ##
//   ##                ##     ##  ##  ##   ### ##    ## ##     ## ##
//   ##        ####### ##     ## #### ##    ##  ######  ##     ## ########
//
// MINIMUM CALIBRATION PAGE
static void handlePageMin(){
  int fail = 0;
  while(boardUIPage == minCalibration){
    /*lcd.clear();
    lcd.print("Press button one");
    lcd.setCursor(0,1);
    lcd.print(" to continue...");
    delay(3000);
    lcd.clear();
    lcd.print("Press button two");
    lcd.setCursor(0,1);
    lcd.print(" to go back...");
    delay(3000);*/
    fail = 0;
    lcd.clear();
    lcd.print("Calibrate min in");
    lcd.setCursor(0, 1);
    lcd.print("3... ");
    delay(1000);
    lcd.print("2... ");
    delay(1000);
    lcd.print("1... ");
    delay(1000);
    lcd.clear();
    lcd.print(" Calibrating... ");
    lcd.setCursor(0, 1);
    lcd.print("please dont move");
    delay(pageDelay);
    for(int i = 0; i < calibrations; i++){
      accelGyroMinCalibration();
      if(!flexMinCalibration()){
        resetFlex();
        resetGyroAccel();
        lcd.clear();
        lcd.print("Min calibration");
        lcd.setCursor(0, 1);
        lcd.print("  unsuccessful ");
        delay(pageDelay);
        lcd.print("   ERROR CODE");
        lcd.setCursor(0, 1);
        lcd.print("       01");
        delay(pageDelay);
        lcd.clear();
        lcd.print("try again   / B2");
        lcd.setCursor(0, 1);
        lcd.print("B1 /    previous");
           
        do{
          button1State = !digitalRead(button1);
          button2State = !digitalRead(button2);
        }while(button1State == 0 && button2State == 0);
        
        if(button1State != 0){
          boardUIPage = minCalibration;
          fail = 1;
          break;
        }else{
          boardUIPage = handOrLetter;
        }
      }
    }
    if(!fail){
      accelGyroMinAverage(calibrations);
      if(flexMinAverage(calibrations)){
        lcd.clear();
        lcd.print("Min calibration");
        lcd.setCursor(0, 1);
        lcd.print("   successful  ");
        delay(pageDelay);
        lcd.clear();
        lcd.print("continue    / B2");
        lcd.setCursor(0, 1);
        lcd.print("B1 /    previous");
      
        do{
          button1State = !digitalRead(button1);
          button2State = !digitalRead(button2);
        }while(button1State == 0 && button2State == 0);
        
        if(button1State != 0){
          boardUIPage = maxCalibration;
        }else{
          boardUIPage = handOrLetter;
        }
      }else{
        resetFlex();
        resetGyroAccel();
        lcd.clear();
        lcd.print("Min calibration");
        lcd.setCursor(0, 1);
        lcd.print("  unsuccessful ");
        delay(pageDelay);
        lcd.print("   ERROR CODE");
        lcd.setCursor(0, 1);
        lcd.print("       02");
        delay(pageDelay);
        lcd.clear();
        lcd.print("try again   / B2");
        lcd.setCursor(0, 1);
        lcd.print("B1 /    previous");
           
        do{
          button1State = !digitalRead(button1);
          button2State = !digitalRead(button2);
        }while(button1State == 0 && button2State == 0);
        
        if(button1State != 0){
          boardUIPage = minCalibration;
          fail = 1;
          break;
        }else{
          boardUIPage = handOrLetter;
        }
      }
    }
  }
}

// DEPRECATED
/*static void handlePageMinCalibration()
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
}*/

//   ########          ##     ##    ###    ##     ##  ######     ###    ##
//   ##     ##         ###   ###   ## ##    ##   ##  ##    ##   ## ##   ##
//   ##     ##         #### ####  ##   ##    ## ##   ##        ##   ##  ##
//   ########          ## ### ## ##     ##    ###    ##       ##     ## ##
//   ##                ##     ## #########   ## ##   ##       ######### ##
//   ##                ##     ## ##     ##  ##   ##  ##    ## ##     ## ##
//   ##        ####### ##     ## ##     ## ##     ##  ######  ##     ## ########
//
// MAXIMUM CALIBRATION PAGE
static void handlePageMax(){
  int fail = 0;
  while(boardUIPage == maxCalibration){
    /*lcd.clear();
    lcd.print("Press button one");
    lcd.setCursor(0,1);
    lcd.print(" to continue...");
    delay(3000);
    lcd.clear();
    lcd.print("Press button two");
    lcd.setCursor(0,1);
    lcd.print(" to go back...");
    delay(3000);*/
    fail = 0;
    lcd.clear();
    lcd.print("Calibrate max in");
    lcd.setCursor(0, 1);
    lcd.print("3... ");
    delay(1000);
    lcd.print("2... ");
    delay(1000);
    lcd.print("1... ");
    delay(1000);
    lcd.clear();
    lcd.print(" Calibrating... ");
    lcd.setCursor(0, 1);
    lcd.print("please dont move");
    delay(pageDelay);
    for(int i = 0; i < calibrations; i++){
      accelGyroMaxCalibration();
      if(!flexMaxCalibration()){
        resetFlex();
        resetGyroAccel();
        lcd.clear();
        lcd.print("Max calibration");
        lcd.setCursor(0, 1);
        lcd.print("  unsuccessful ");
        delay(pageDelay);
        lcd.print("   ERROR CODE");
        lcd.setCursor(0, 1);
        lcd.print("       01");
        delay(pageDelay);
        lcd.clear();
        lcd.print("try again   / B2");
        lcd.setCursor(0, 1);
        lcd.print("B1 /    previous");
           
        do{
          button1State = !digitalRead(button1);
          button2State = !digitalRead(button2);
        }while(button1State == 0 && button2State == 0);
        
        if(button1State != 0){
          boardUIPage = maxCalibration;
          fail = 1;
          break;
        }else{
          boardUIPage = minCalibration;
        }
      }
    }
    if(!fail){
      accelGyroMaxAverage(calibrations);
      if(flexMaxAverage(calibrations)){
        lcd.clear();
        lcd.print("Max calibration");
        lcd.setCursor(0, 1);
        lcd.print("   successful  ");
        delay(pageDelay);
        lcd.clear();
        lcd.print("continue    / B2");
        lcd.setCursor(0, 1);
        lcd.print("B1 /    previous");
      
        do{
          button1State = !digitalRead(button1);
          button2State = !digitalRead(button2);
        }while(button1State == 0 && button2State == 0);
        
        if(button1State != 0){
          if(fullHand == 0){
            boardUIPage = interpretation;
          }else{
            boardUIPage = letterCalibration;
          }
        }else{
          boardUIPage = minCalibration;
        }
      }else{
        resetFlex();
        resetGyroAccel();
        lcd.clear();
        lcd.print("Max calibration");
        lcd.setCursor(0, 1);
        lcd.print("  unsuccessful ");
        delay(pageDelay);
        lcd.print("   ERROR CODE");
        lcd.setCursor(0, 1);
        lcd.print("       02");
        delay(pageDelay);
        lcd.clear();
        lcd.print("try again   / B2");
        lcd.setCursor(0, 1);
        lcd.print("B1 /    previous");
           
        do{
          button1State = !digitalRead(button1);
          button2State = !digitalRead(button2);
        }while(button1State == 0 && button2State == 0);
        
        if(button1State != 0){
          boardUIPage = maxCalibration;
          fail = 1;
          break;
        }else{
          boardUIPage = minCalibration;
        }
      }
    }
  }
}

// DEPRECATED
/*static void handlePageMax(){
  int fail = 0;
  while(boardUIPage == maxCalibration){
    lcd.clear();
    lcd.print("Calibrate max in");
    lcd.setCursor(0, 1);
    lcd.print("3... ");
    delay(1000);
    lcd.print("2... ");
    delay(1000);
    lcd.print("1... ");
    delay(1000);
    lcd.clear();
    lcd.print(" Calibrating... ");
    lcd.setCursor(0, 1);
    lcd.print("please dont move");
    delay(pageDelay);
    lcd.clear();
    lcd.print("Press button one");
    lcd.setCursor(0,1);
    lcd.print(" to continue...");
    delay(3000);
    lcd.clear();
    lcd.print("Press button two");
    lcd.setCursor(0,1);
    lcd.print(" to go back...");
    delay(3000);
    if(1 == 1){
      lcd.clear();
      lcd.print("Max calibration");
      lcd.setCursor(0, 1);
      lcd.print("   successful  ");
      delay(pageDelay);
      lcd.clear();
      lcd.print("continue    / B2");
      lcd.setCursor(0, 1);
      lcd.print("B1 /    previous");
      
      do{
        button1State = !digitalRead(button1);
        button2State = !digitalRead(button2);
      }while(button1State == 0 && button2State == 0);
      
      if(button1State != 0){
        if(fullHand == 0){
          boardUIPage = interpretation;
        }else{
          boardUIPage = letterCalibration;
        }
      }else{
        boardUIPage = minCalibration;
      }
    }else{
      lcd.clear();
      lcd.print("Max calibration");
      lcd.setCursor(0, 1);
      lcd.print("  unsuccessful ");
      delay(pageDelay);
      lcd.clear();
      lcd.print("try again   / B2");
      lcd.setCursor(0, 1);
      lcd.print("B1 /    previous");
      
      do{
        button1State = !digitalRead(button1);
        button2State = !digitalRead(button2);
      }while(button1State == 0 && button2State == 0);
      
      if(button1State != 0){
        boardUIPage = maxCalibration;
      }else{
        boardUIPage = minCalibration;
      }
    }
  }
}*/

// DEPRECATED
/*static void handlePageMaxCalibration()
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
}*/

//   ########          ##       ######## ########  ######     ###    ##
//   ##     ##         ##       ##          ##    ##    ##   ## ##   ##
//   ##     ##         ##       ##          ##    ##        ##   ##  ##
//   ########          ##       ######      ##    ##       ##     ## ##
//   ##                ##       ##          ##    ##       ######### ##
//   ##                ##       ##          ##    ##    ## ##     ## ##
//   ##        ####### ######## ########    ##     ######  ##     ## ########
//
// LETTER CALIBRATION PAGE

static void handlePageLet(){
  while(boardUIPage == letterCalibration){
    lcd.clear();
    lcd.print(" Calibration of ");
    lcd.setCursor(0, 1);
    lcd.print("letters requires");
    delay(pageDelay);
    lcd.clear();
    lcd.print("  confirmation  ");
    lcd.setCursor(0, 1);
    lcd.print("after each step ");
    delay(pageDelay);
    lcd.clear();
    lcd.print("confirm     / B2");
    lcd.setCursor(0, 1);
    lcd.print("B1 /     refresh");
    delay(pageDelay);
    lcd.clear();
    lcd.print(" Calibrating: ");
    lcd.setCursor(0, 1);
    lcd.print("Current value: ");
    for(int i = 0; i < 26; i++){
      lcd.setCursor(14, 0);
      lcd.print(alph.charAt(i));
      int check = 0;
      while(!check){
        delay(500);
        lcd.setCursor(15, 1);
        if(flexLetterCalibration(i)){
          lcd.print(translate(flexRead(), accelGyroRead())); // pass i and get return as translated
          while(digitalRead(button2) != 0){
            if(digitalRead(button1) == 0){
              check = 1;
              break;
            }
          }
        }else{
          lcd.print(0);
        }
      }
    }
    /*lcd.clear();
    lcd.print("Press button one");
    lcd.setCursor(0,1);
    lcd.print(" to continue...");
    delay(3000);
    lcd.clear();
    lcd.print("Press button two");
    lcd.setCursor(0,1);
    lcd.print(" to go back...");
    delay(3000);*/
    lcd.clear();
    lcd.print("Let calibration");
    lcd.setCursor(0, 1);
    lcd.print("   successful  ");
    delay(pageDelay);
    lcd.clear();
    lcd.print("continue    / B2");
    lcd.setCursor(0, 1);
    lcd.print("B1 /    previous");
    
    do{
      button1State = !digitalRead(button1);
      button2State = !digitalRead(button2);
    }while(button1State == 0 && button2State == 0);
    
    if(button1State != 0){
      boardUIPage = interpretation;
    }else{
      boardUIPage = maxCalibration;
    }
  }
}

//   ########          #### ##    ## ######## ######## ########  ########
//   ##     ##          ##  ###   ##    ##    ##       ##     ## ##     ##
//   ##     ##          ##  ####  ##    ##    ##       ##     ## ##     ##
//   ########           ##  ## ## ##    ##    ######   ########  ########
//   ##                 ##  ##  ####    ##    ##       ##   ##   ##
//   ##                 ##  ##   ###    ##    ##       ##    ##  ##
//   ##        ####### #### ##    ##    ##    ######## ##     ## ##
//
// INTERPRETATION PAGE
static void handlePageInt(){
  int lastPos = 0;
  int oneOnePos = 0;
  int oneTwoPos = 0;
  int twoOnePos = 0;
  int twoTwoPos = 0;
  while(boardUIPage == interpretation){
    lcd.clear();

    // If length is larger than or equal to 31 (lcd size - 1 for blinking char)
    // print last 31 chars of result
    if(result.length() > 30){
      twoTwoPos = result.length();
      twoOnePos = result.length() - 15;
      oneTwoPos = result.length() - 15;
      oneOnePos = result.length() - 15 - 16;
      lastPos = 31;
    }else if(result.length() > 15 && result.length() <= 30){
      twoTwoPos = result.length();
      twoOnePos = 16;
      oneTwoPos = 16;
      oneOnePos = 0;
      lastPos = result.length();
    }else{
      twoTwoPos = 0;
      twoOnePos = 0;
      oneTwoPos = result.length();
      oneOnePos = 0;
      lastPos = result.length();
    }

    if(oneOnePos + oneTwoPos != 0){
      lcd.setCursor(0, 0);
      lcd.print(result.substring(oneOnePos, oneTwoPos));
    }
    if(twoOnePos + twoTwoPos != 0){
      lcd.setCursor(0, 1);
      lcd.print(result.substring(twoOnePos, twoTwoPos));
    }
    
    int check = 0;
    while(!check){
      delay(500);
      lcd.setCursor(lastPos % 16, lastPos / 16);
      char temp = translate(flexRead(), accelGyroRead());
      lcd.print(temp); // get translated value
      while(digitalRead(button2) != 0){
        lcd.setCursor(lastPos % 16, lastPos / 16);
        //lcd.cursor();
        lcd.blink();
        if(digitalRead(button1) == 0){
          result += temp;
          check = 1;
          break;
        }
      }
    }
    
    //delay(10000);
    
    // Initial print
    /*if(result.length() % 16 != 0){
      if(result.length() > 16){
        lcd.setCursor(0, 1);
        lastPos = 16 + (result.length() % 16) + 1;
      }else{
        lcd.setCursor(0, 0);
        lastPos = (result.length() % 16) + 1;
      }
      lcd.print(result.substring(result.length() - (result.length() % 16)));
    }
    
    for(int i = 0; i < 2; i++){
      if(result.length() % 16 == 0){
        lcd.setCursor(0, 1);
        lcd.print(result.substring(((result.length() / 16) - 2) * 16), ((result.length() / 16) - 1) * 16));
      }
    }*/
  }
}

// DEPRECATED
/*static void handlePageInterpretation(){
  if(tickCounter == 1){
    Serial.println("Current interpretation: ");
  }
  

  char interpretedLetter = translate(flexRead(), accelGyroRead());
  
  Serial.println(interpretedLetter);

  // FOLLOWING DELAY ONLY FOR DEBUGGING PURPOSES
  delay(100);
}*/

//   ######## ####  ######  ##    ##
//      ##     ##  ##    ## ##   ##
//      ##     ##  ##       ##  ##
//      ##     ##  ##       #####
//      ##     ##  ##       ##  ##
//      ##     ##  ##    ## ##   ##
//      ##    ####  ######  ##    ##
//
// LOOP
void SerialTick(){
  switch(boardUIPage){
    case welcome:
      handlePageWel();
      break;
    case handOrLetter:
      handlePageOrr();
      break;
    case minCalibration:
      handlePageMin();
      break;
    case maxCalibration:
      handlePageMax();
      break;
    case letterCalibration:
      handlePageLet();
      break;
    case interpretation:
      handlePageInt();
      break;
  }
}

