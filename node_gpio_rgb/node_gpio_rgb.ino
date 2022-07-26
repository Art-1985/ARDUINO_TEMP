#include  "node_mcu_gpio.h"


void setup() {
  init_rgb_blink();
}

void loop() {
  run_rgb_blink();
}

void init_rgb_blink(void){
  pinMode(GLED,OUTPUT);
  pinMode(RLED,OUTPUT);
  pinMode(BLED,OUTPUT);
  digitalWrite(GLED,LOW);
  digitalWrite(RLED,LOW);
  digitalWrite(BLED,LOW);  
}
void run_rgb_blink(void){
  digitalWrite(GLED,HIGH);
  delay(1000);
  digitalWrite(GLED,LOW);
  delay(1000);
  digitalWrite(RLED,HIGH);
  delay(1000);
  digitalWrite(RLED,LOW);
  delay(1000);
  digitalWrite(BLED,HIGH);
  delay(1000);
  digitalWrite(BLED,LOW); 
   delay(1000); 
}


void init_key_light(void){
  pinMode(GLED,OUTPUT);
  pinMode(KEYIN,INPUT);
  digitalWrite(GLED,LOW);
}
void run_key_light(void){
  if(digitalRead(KEYIN)==0)
    digitalWrite(GLED,HIGH);
  else
    digitalWrite(GLED,LOW);
  delay(100);
}


