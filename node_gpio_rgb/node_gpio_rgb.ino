#include  "node_mcu_gpio.h"


void setup() {
  // put your setup code here, to run once:
  //pinMode(GLED,OUTPUT);
  //pinMode(RLED,OUTPUT);
  //pinMode(BLED,OUTPUT);
  //digitalWrite(GLED,LOW);
  //digitalWrite(RLED,LOW);
  //digitalWrite(BLED,LOW);
  init_rgb_blink();
}

void loop() {
  // put your main code here, to run repeatedly:
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


/*
node_gpio_input
void setup() {
  pinMode(GLED,OUTPUT);
  pinMode(KEYIN,INPUT);
  digitalWrite(GLED,LOW);
}

void loop() {
  if(digitalRead(KEYIN)==0)
    digitalWrite(GLED,HIGH);
  else
    digitalWrite(GLED,LOW);
  delay(100);
}
*/
