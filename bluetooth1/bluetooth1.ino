#include <SoftwareSerial.h> // import the serial library
SoftwareSerial BluetoothModule(4, 5); // RX, TX
int LED = 13;

void setup() {
 pinMode(LED, OUTPUT);
 BluetoothModule.begin(38400); /* Define baud rate for serial communication */
 Serial.begin(115200);
}

void loop() {

 if (BluetoothModule.available())  /* If data is available on serial port */
    {
      char data_received; 
      data_received = BluetoothModule.read();  /* Data received from bluetooth */
      if (data_received == '1')
      {
       digitalWrite(LED, HIGH);
       Serial.write("LED turned ON\n"); 
       BluetoothModule.write("LED turned ON\n");       
      }
      else if (data_received == '2')
      {
       digitalWrite(LED, LOW);
       Serial.write("LED turned OFF\n");
       BluetoothModule.write("LED turned OFF\n");
      }
      else
      {
       Serial.write("Select either 1 or 2->");
       BluetoothModule.write("Select either 1 or 2");
      }
    }
}
