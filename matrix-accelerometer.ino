#include "LedControl.h"
#include <Wire.h>

LedControl lm = LedControl(12, 10, 11, 1);

unsigned long delaytime = 50;

int x = 5;
int y = 5;

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup(){
  clear();

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  
}

void loop(){
  clear();
  lm.setLed(0, x, y, true);

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers

  GyX = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)

  x = map(GyY, 8000, -8000, 0, 7);
  y = map(GyX, -8000, 8000, 0, 7);
  
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);
  
  delay(50);
}

void clear(){
  lm.shutdown(0, false);
  lm.setIntensity(0, 6);
  lm.clearDisplay(0);
}
