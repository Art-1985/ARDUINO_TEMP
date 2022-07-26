#include  "node_mcu_gpio.h"

void init_rgb_blink(void){
    pinMode(GLED,OUTPUT);
    pinMode(RLED,OUTPUT);
    pinMode(BLED,OUTPUT);
    digitalWrite(GLED,LOW);
    digitalWrite(RLED,LOW);
    digitalWrite(BLED,LOW);  
}

