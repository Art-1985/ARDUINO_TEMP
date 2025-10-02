#include "Arduino.h"
#include "node_mcu_dht.h"
#include  "DHTesp.h"

DHTesp dht11_sensor;

//-- init_dht11_sense ----------------------------------------------------
void init_dht11_sense(void) {
    dht11_sensor.setup(5, DHTesp::DHT11); // Connect DHT11 to GPIO 5
    Serial.println("DHT11 Ready!");
    delay(2000);
}


float get_dht_humi(void)
{
    float  temp_var;
    temp_var = dht11_sensor.getHumidity();
    delay(100);
    return    temp_var;
}

float get_dht_temp(void)
{
    float  temp_var;
    temp_var = dht11_sensor.getTemperature();
    delay(100);
    return    temp_var;
}
