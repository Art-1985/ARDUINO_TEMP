#include  "node_mcu_gpio.h"


const int analogInPin = A0;
const int analogOutPin = 13;

int sensorValue = 0;  
int outputValue = 0; 


void setup() {
  init_rgb_blink();
  Serial.begin(115200);
}

void loop() {
  run_rgb_blink();

  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(analogOutPin, outputValue);

  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);
  //delay(2);
}


//-- Blink LED Declarations and Function -------------------------------------
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

//-- Light LED By Key Declarations and Function ------------------------------
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


