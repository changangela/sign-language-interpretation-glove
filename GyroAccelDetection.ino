//////Wire Setup for Tiva C//////
  //VCC to 3.3V
  //GND to GND
  //SCL to PA6 (SCL pin on Tiva C)
  //SDA to PA7 (SDA pin on Tiva C)
  //INT to PA5
//////////////////////////////////
int i;

#include <I2Cdev.h>
#include <helper_3dmath.h>
#include <MPU6050.h>
//#include <MPU6050_6Axis_MotionApps20.h>
//#include <MPU6050_9Axis_MotionApps41.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#endif

MPU6050 accelgyro;

const int x = 0;
const int y = 1;
const int z = 2;

int16_t accel[3] = {0};
int16_t accelMin[3] = {0};
int16_t accelMax[3] = {0};

int16_t gyro[3] = {0};
int16_t gyroMin[3] = {0};
int16_t gyroMax[3] = {0};

#define OUTPUT_READABLE_ACCELGYRO

#define LED_PIN 13
bool blinkState = false;


void resetGyroAccel(){
  // resets all values to default
  for(int i  = 0; i < 3; ++i){
    gyro[i] = 0;
    gyroMin[i] = 0;
    gyroMax[i] = 0;
    accel[i] = 0;
    accelMin[i] = 0;
    accelMax[i] = 0;
  }
}

void GyroAccelInit() {
  //Wire.begin();
  Wire.setModule(1);
  Wire.begin();
  
  accelgyro.initialize();

  //Used to calibrate the gyro and accel, by using an offset
  accelgyro.setXAccelOffset(-2196);
  accelgyro.setYAccelOffset(3286);
  accelgyro.setZAccelOffset(1460);
  accelgyro.setXGyroOffset(66);
  accelgyro.setYGyroOffset(-13);
  accelgyro.setZGyroOffset(-6);
  //
 
  pinMode(LED_PIN, OUTPUT);
}

void accelGyroMinCalibration() {
 
  //accelgyro.getMotion6(&ax, &ay, &az,&gx, &gy, &gz);
  //accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  int16_t tempAccelMin[3];
  accelgyro.getRotation(&tempAccelMin[x], &tempAccelMin[y], &tempAccelMin[z]); // Gives you rotation ???
  //accelgyro.getRotation(&gx, &gy, &gz); //Gives you acceleration ??!

  for(int i = 0; i < 3; ++i){
    // keep a running average of the calibration for a certain number of ticks
    accelMin[i] += tempAccelMin[i];
  }
  

#ifdef OUTPUT_READABLE_ACCELGYRO
    // display tab-separated accel/gyro x/y/z values
    
    Serial.print("Accelerometer");
    Serial.print(accelMin[x]); Serial.print(" || ");
    Serial.print(accelMin[y]); Serial.print(" || ");
    Serial.println(accelMin[z]);
//    Serial.print(gx); Serial.print(" || ");
//    Serial.print(gy); Serial.print(" || ");
//    Serial.println(gz);
    
#endif


// blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
}

void accelGyroMaxCalibration() {
 
  //accelgyro.getMotion6(&ax, &ay, &az,&gx, &gy, &gz);
  //accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  int16_t tempAccelMax[3];
  accelgyro.getRotation(&tempAccelMax[x], &tempAccelMax[y], &tempAccelMax[z]); // Gives you rotation ???
  //accelgyro.getRotation(&gx, &gy, &gz); //Gives you acceleration ??!

  for(int i = 0; i < 3; ++i){
    // keep a running average of the calibration for a certain number of ticks
    accelMax[i] += tempAccelMax[i];
  }
  

#ifdef OUTPUT_READABLE_ACCELGYRO
    // display tab-separated accel/gyro x/y/z values
    
    Serial.print("Accelerometer");
    Serial.print(accelMax[x]); Serial.print(" || ");
    Serial.print(accelMax[y]); Serial.print(" || ");
    Serial.println(accelMax[z]);
//    Serial.print(gx); Serial.print(" || ");
//    Serial.print(gy); Serial.print(" || ");
//    Serial.println(gz);
    
#endif


// blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
}

void accelGyroMinAverage(int numReadings){
  // takes the average reading for better results
  for(int i = 0; i < 3; ++i){
    accelMin[i] /= numReadings;
  }

  Serial.print("Accelerometer min average: ");
  Serial.print(accelMin[x]); Serial.print(" || ");
  Serial.print(accelMin[y]); Serial.print(" || ");
  Serial.println(accelMin[z]);
    
}

void accelGyroMaxAverage(int numReadings){
  // takes the average reading for better results
  for(int i = 0; i < 3; ++i){
    accelMax[i] /= numReadings;
  }

  Serial.print("Accelerometer max average");
  Serial.print(accelMax[x]); Serial.print(" || ");
  Serial.print(accelMax[y]); Serial.print(" || ");
  Serial.println(accelMax[z]);
    
}

