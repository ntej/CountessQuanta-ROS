/*Author : NavatejaReddy*/
#include <Wire.h>

#define DEVICE_ADDRESS 0x05
#define M1_MODE_REGISTER 0x44
#define M2_MODE_REGISTER 0x47
#define M1_POWER_REGISTER 0x45
#define M2_POWER_REGISTER 0x46



void setup() {
  
 Wire.begin();
  Serial.begin(9600);

  Wire.beginTransmission(DEVICE_ADDRESS); //connecting to slave i.e DC motor controller
  Wire.write(M1_MODE_REGISTER); //Writing to mode register, i.e power mode
  Wire.write(0x00); 
  Wire.endTransmission(0);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(M1_POWER_REGISTER); 
  Wire.write(80);
  Wire.endTransmission(0); 
  delay(100);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(M1_POWER_REGISTER); 
  Wire.write(0);
  Wire.endTransmission(0); 
  //delay(200);
}

void loop() {
  
  int potentioMeterValue = analogRead(A6);
  Serial.println(potentioMeterValue);
  delay(1); 
}



