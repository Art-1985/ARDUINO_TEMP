#include "Wire.h"

byte byteVar = 10;

void setup(){
  Serial.begin(115200); 
  while(!Serial){} // Waiting for serial connection
  Serial.println();
  Serial.println("Start I2C scanner ...");
  Serial.print("\r\n");
  byte count = 0;
  
  Wire.begin();
  for (byte i = 8; i < 120; i++){
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0){
      Serial.print("Found I2C Device: ");
      Serial.print(" (0x");
      Serial.print(i, HEX);
      Serial.println(")");
      count++;
      delay(1);} }//End of for-loop
  Serial.print("\r\n");
  Serial.println("Finish I2C scanner");
  Serial.print("Found ");
  Serial.print(count, HEX);
  Serial.println(" Device(s).");
} //End of set-up

void loop() {
  Wire.beginTransmission(44);
  Wire.write(byteVar);
  Wire.endTransmission();
  byteVar++;
  //if (byteVar == 64){ byteVar = 0; }
  //if (Wire.endTransmission() == 0){
        //delay(1); }
  delay(500);
  }
