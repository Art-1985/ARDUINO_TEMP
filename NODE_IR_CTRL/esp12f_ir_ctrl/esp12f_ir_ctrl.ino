
//IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
//An IR detector/demodulator must be connected to the input RECV_PIN.

#ifndef UNIT_TEST
  #include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#define RED_BUT   0xFF22DD
#define GRE_BUT   0xFF02FD
#define BLU_BUT   0xFFC23D

#define RED_LED   12
#define GRE_LED   13
#define BLU_LED   14 

// IR detector/demodulator is connected to GPIO pin
uint16_t RECV_PIN = 4; //IR_R_IO_PIN

IRrecv irrecv(RECV_PIN);
decode_results results; //IR解碼解果

//====== Initialize ======//
void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // Start the receiver
  pinMode(RED_LED,OUTPUT);
  pinMode(GRE_LED,OUTPUT);
  pinMode(BLU_LED,OUTPUT);
  digitalWrite(RED_LED,LOW);
  digitalWrite(GRE_LED,LOW);
  digitalWrite(BLU_LED,LOW); 
}

//====== Back-ground Loop ======//
void loop() {
  if (irrecv.decode(&results)) {
    serialPrintUint64(results.value, HEX);
    Serial.println(" ");
    irrecv.resume();  // Receive the next value
    }
  if (results.value == RED_BUT){
    digitalWrite(RED_LED,HIGH);}
  else{digitalWrite(RED_LED,LOW);}
  
  if (results.value == GRE_BUT){
    digitalWrite(GRE_LED,HIGH);}
  else{digitalWrite(GRE_LED,LOW);}
  
  if (results.value == BLU_BUT){
    digitalWrite(BLU_LED,HIGH);}
  else{digitalWrite(BLU_LED,LOW);}
  delay(1000);
}
