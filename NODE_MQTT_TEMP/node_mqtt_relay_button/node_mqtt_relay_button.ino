//=========================================================================
//  FILE:     node_mqtt_relay_button.ioc
//  Target:   NodeMCU_1.0(ESP-12E)
//  Author:   Arthur (Lu Hungche)
//  Date:     2021/01/02
//-------------------------------------------------------------------------
//  Description: 連接到MQTT伺服,
//      ON  : "0"_RelayOFF, "1"_RelayON, "2"_RelayON
//      SEND: "0"_RelayOFF, "1"_RelayON
//      It will reconnect to the server if the connection is lost using a blocking
//      reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
//      achieve the same result without blocking the main loop.
//      The current state is stored in EEPROM and restored on bootup
//-------------------------------------------------------------------------
// History:
// Ver  | dd-mmm-yyyy |  Who  | Description of changes
// =====|=============|=======|============================================
//  0.1 | 03 Jun 2021 | Arthur| Original Release.
// -----|-------------|-------|--------------------------------------------
//  0.2 |
// -----|-------------|-------|--------------------------------------------
//=========================================================================
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Bounce2.h>
#include <EEPROM.h>

//===== 參數宣告 ===========================//
const char* ssid = "Art. XR";
const char* password = "abcdef12";
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* outTopic = "artTopic";
const char* inTopic = "artInTopic";

//===== 變數宣告 ===========================//
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

int   relay_pin   = 16;
int   button_pin  = 0;
int   led_pin     = 13;
bool  relayState  = HIGH;

//===== 物件宣告 ===========================//
Bounce debouncer = Bounce();

//===== 初始化程序 ===========================//

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);  }
  Serial.println();

  // Switch RELAY if  was received as first character
  if ((char)payload[0] == '0') {
    digitalWrite(relay_pin, LOW);   // Turn the LED on (Note that LOW is the voltage level
    Serial.println("relay_pin -> LOW");
    relayState = LOW;
    //EEPROM.write(0, relayState);    // Write state to EEPROM
    //EEPROM.commit();
  } else if ((char)payload[0] == '1') {
    digitalWrite(relay_pin, HIGH);  // Turn the LED off by making the voltage HIGH
    Serial.println("relay_pin -> HIGH");
    relayState = HIGH;
    //EEPROM.write(0, relayState);    // Write state to EEPROM
    //EEPROM.commit();
  } else if ((char)payload[0] == '2') {
    relayState = !relayState;
    digitalWrite(relay_pin, relayState);  // Turn the LED off by making the voltage HIGH
    Serial.print("relay_pin -> switched to ");
    Serial.println(relayState);
    //EEPROM.write(0, relayState);    // Write state to EEPROM
    //EEPROM.commit();
  }
}

void setup() {
  //EEPROM.begin(512);              // Begin eeprom to store on/off state
  set_up_gpio();
  //relayState = EEPROM.read(0);
  relayState = HIGH;
  digitalWrite(relay_pin, relayState);
  debouncer.attach(button_pin);   // Use the bounce2 library to debounce the built in button
  debouncer.interval(50);         // Input must be low for 50 ms

  digitalWrite(led_pin, LOW);          // Blink to indicate setup
  delay(500);
  digitalWrite(led_pin, HIGH);
  delay(500);
  Serial.begin(115200);
  set_up_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


//===== 背景程序 ===========================//
void loop() {
  if (!client.connected()) {
    run_wifi_connect(); }
  client.loop();
  run_push_detect();
}


//==================== 初始副程序 =========================================//
void set_up_gpio(){
  pinMode(relay_pin, OUTPUT);
  pinMode(button_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  }

void set_up_uart(){
  }

void set_up_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    run_push_detect();
    for (int i = 0; i < 500; i++) {
      run_push_detect();
      delay(1); }
    Serial.print(".");  }
  digitalWrite(led_pin, LOW);
  delay(500);
  digitalWrite(led_pin, HIGH);
  delay(500);
  digitalWrite(led_pin, LOW);
  delay(500);
  digitalWrite(led_pin, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  }



//==================== 副程序 ============================================//

//===== 外部按鈕觸發 ======================//
//濾彈跳並取FallingEdge(過濾彈跳)
void run_push_detect() {
  debouncer.update();
  if ( debouncer.fell() ) { //如果按下按鈕，執行此段程式
    Serial.println("Debouncer fell, falling edge");
    relayState = !relayState; //Toggle Relay
    digitalWrite(relay_pin, relayState);
    // EEPROM.write(0, relayState);    // Write state to EEPROM
    if (relayState == 1) {
      client.publish(outTopic, "1");  }
    else if (relayState == 0) {
      client.publish(outTopic, "0");  }
  } //End of "debouncer.fell()"
}

//===== wifi 連接 =======================//
void run_wifi_connect() {
  String clientId = "ESP8266Client-";
  while (!client.connected()) { //嘗試連接wifi
    Serial.print("Attempting MQTT connection...");
    clientId += String(random(0xffff), HEX);  //Random client ID
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "Sonoff1 booted");
      // ... and resubscribe
      client.subscribe(inTopic);  } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      for (int i = 0; i < 5000; i++) {
        run_push_detect();
        delay(1); } }
  } // End of "!client.connected()"
}

//=====  連接 =======================//
void run_delay_sec( unsigned int cntVar){
  for (int i = 0; i < 5000; i++) {
    delay(1); }
  }
