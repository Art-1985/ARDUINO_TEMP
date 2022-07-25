#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#define LED_BUILTIN 13
#define LED2_BUILTIN 12
#define LED3_BUILTIN 15
//SSID and Password of your WiFi router
const char* ssid = "C301";
const char* password = "ilovedmt";
int stateLED = LOW;
int stateLED2 = LOW;
int stateLED3 = LOW;
//Static IP address configuration
IPAddress ip(192, 168, 1, 6); //ESP static ip
IPAddress gateway(192, 168, 1, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS
ESP8266WebServer server(80);

void handleRoot() {
    response();
}

void handleLedOn() {
  stateLED = HIGH;
  digitalWrite(LED_BUILTIN, stateLED);
  response();
}
void handleLed2On() {
  stateLED2 = HIGH;
  digitalWrite(LED2_BUILTIN, stateLED2);
  response();
}
void handleLed3On() {
  stateLED3 = HIGH;
  digitalWrite(LED3_BUILTIN, stateLED3);
  response();
}
void handleLedOff() {
  stateLED = LOW;
  digitalWrite(LED_BUILTIN, stateLED);
  response();
}
void handleLed2Off() {
  stateLED2 = LOW;
  digitalWrite(LED2_BUILTIN, stateLED2);
  response();
}
void handleLed3Off() {
  stateLED3 = LOW;
  digitalWrite(LED3_BUILTIN, stateLED3);
  response();
}
void handleAllOff() {
  stateLED = LOW;
  digitalWrite(LED_BUILTIN, stateLED);
  stateLED2 = LOW;
  digitalWrite(LED2_BUILTIN, stateLED2);
  stateLED3 = LOW;
  digitalWrite(LED3_BUILTIN, stateLED3);
  response();
}
const String HtmlHtml = "<html><head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />"
    "<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"/></head>";
const String HtmlHtmlClose = "</html>\n";
const String HtmlTitle = "<h1>ESP8266 測試AP WebServer exercise</h1><br/>\n";
const String HtmlLedStateLow = "<big>LED is now <b>OFF</b></big><br/>\n";
const String HtmlLedStateHigh = "<big>LED is now <b>ON</b></big><br/>\n";
const String HtmlLed2StateLow = "<big>LED2 is now <b>OFF</b></big><br/>\n";
const String HtmlLed2StateHigh = "<big>LED2 is now <b>ON</b></big><br/>\n";
const String HtmlLed3StateLow = "<big>LED3 is now <b>OFF</b></big><br/>\n";
const String HtmlLed3StateHigh = "<big>LED3 is now <b>ON</b></big><br/>\n";
const String HtmlButtons = 
    "<a href=\"LEDOn\"><button style=\"display: block; width: 100%;\">ON</button></a><br/>"
    "<a href=\"LEDOff\"><button style=\"display: block; width: 100%;\">OFF</button></a><br/>"
    "<a href=\"LED2On\"><button style=\"display: block; width: 100%;\">ON</button></a><br/>"
    "<a href=\"LED2Off\"><button style=\"display: block; width: 100%;\">OFF</button></a><br/>"
    "<a href=\"LED3On\"><button style=\"display: block; width: 100%;\">ON</button></a><br/>"
    "<a href=\"LED3Off\"><button style=\"display: block; width: 100%;\">OFF</button></a><br/>"
    "<a href=\"ALLOff\"><button style=\"display: block; width: 100%;\">ALL OFF</button></a><br/>";

void response(){
  String htmlRes = HtmlHtml + HtmlTitle;
  if(stateLED == LOW){
    htmlRes += HtmlLedStateLow;
  }else{
    htmlRes += HtmlLedStateHigh;
  }
  if(stateLED2 == LOW){
    htmlRes += HtmlLed2StateLow;
  }else{
    htmlRes += HtmlLed2StateHigh;
  }
  if(stateLED3 == LOW){
    htmlRes += HtmlLed3StateLow;
  }else{
    htmlRes += HtmlLed3StateHigh;
  }
  htmlRes += HtmlButtons;
  htmlRes += HtmlHtmlClose;

  server.send(200, "text/html", htmlRes);
}

void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println();
   //WiFi.config(ip, gateway, subnet);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
   // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    //If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
    
    server.on("/", handleRoot);
    server.on("/LEDOn", handleLedOn);
    server.on("/LEDOff", handleLedOff);
    server.on("/LED2On", handleLed2On);
    server.on("/LED2Off", handleLed2Off);
    server.on("/LED3On", handleLed3On);
    server.on("/LED3Off", handleLed3Off);
    server.on("/ALLOff", handleAllOff);
    server.begin();
    Serial.println("HTTP server beginned");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, stateLED);
    pinMode(LED2_BUILTIN, OUTPUT);
    digitalWrite(LED2_BUILTIN, stateLED2);
    pinMode(LED3_BUILTIN, OUTPUT);
    digitalWrite(LED3_BUILTIN, stateLED3);
}

void loop() {
    server.handleClient();
}
