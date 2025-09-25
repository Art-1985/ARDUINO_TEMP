#include "Arduino.h"
#include "node_mcu_gpio.h"


void init_rgb_blink(void) {
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  pinMode(BLED, OUTPUT);
  digitalWrite(GLED, LOW);
  digitalWrite(RLED, LOW);
  digitalWrite(BLED, LOW);
}


//-- Blink LED Declarations and Function -------------------------------------
void run_rgb_blink(void) {
  digitalWrite(GLED, HIGH);
  delay(100); // delay 100ms
  digitalWrite(GLED, LOW);
  delay(100);
  digitalWrite(RLED, HIGH);
  delay(100);
  digitalWrite(RLED, LOW);
  delay(100);
  digitalWrite(BLED, HIGH);
  delay(100);
  digitalWrite(BLED, LOW);
  delay(100);
}


