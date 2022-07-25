#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <WebSocketsServer.h>

WebSocketsServer webSocket = WebSocketsServer(81);
const char *ssid = “TCSF001";
const char *password = “12345678";

void setup(){
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    switch(type) {
        case WStype_DISCONNECTED:
              break;
        case WStype_CONNECTED:
              IPAddress ip = webSocket.remoteIP(num);
              break;
        case WStype_TEXT:
             String text = String((char *) &payload[0]);
             if(text=="LED"){
                  digitalWrite(13,HIGH);
                  delay(500);
                  digitalWrite(13,LOW);
                  Serial.println("led just lit");
                  webSocket.sendTXT(num, "led just lit", lenght);
             }
             if(text.startsWith("x")){
               String xVal=(text.substring(text.indexOf("x")+1,text.length())); 
               int xInt = xVal.toInt();
               analogWrite(redPin,xInt); 
               Serial.println(xVal);
               webSocket.sendTXT(num, "red changed", lenght);
            }
           if(text=="RESET"){
             analogWrite(bluePin,LOW);
             analogWrite(redPin,LOW);
             analogWrite(greenPin,LOW);
             Serial.println("reset");
           }
           webSocket.sendTXT(num, payload, lenght);
           webSocket.broadcastTXT(payload, lenght);
           break;
        case WStype_BIN:
           hexdump(payload, lenght);
         // echo data back to browser
          webSocket.sendBIN(num, payload, lenght);
          break;
    }
}

