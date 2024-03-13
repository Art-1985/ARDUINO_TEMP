#include  "node_mcu_gpio.h"
#include  <ESP8266WiFi.h>
//#include  <WiFiClient.h>
#include  "DHTesp.h"
#include  <TridentTD_LineNotify.h>

// #define SSID    "Art. Pro"
// #define PASS    "abcdef12"
#define SSID    "Art.K4"
#define PASS    "0933547825"
#define HOST    "api.thingspeak.com" // ThingSpeak IP Address: 184.106.153.149
#define PORT    80

#define LINE_TOKEN "or8MjvO4AsC4iZth7EA1oeafQV0BDCtbC4MIhNxuxUl"

String GET = "GET /update?key=A7FWFGRLU2P1MOK9";
DHTesp dht11_sensor;


float temperature, humidity;
const int analogInPin = A0;
const int analogOutPin = 13;

int sensorValue = 0;
int outputValue = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\nESP8266 Ready!");

  init_wifi_link();
  init_dht11_sense();
  init_rgb_blink();
  delay(100);
}

void loop() {
  humidity = dht11_sensor.getHumidity();
  temperature = dht11_sensor.getTemperature();


  // 確認取回的溫溼度數據可用(檢視是否為NULL)
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read form DHT11");
    return;
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(", Temperature: ");
    Serial.println(temperature);
    updateDHT11(); // Upload to Thingspeak (DHT11 Temp. & Hum.)
    updateLine();
  }

  delay(5000);  // Wait a bit before scanning again
  run_rgb_blink();

  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(analogOutPin, outputValue);

  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);
}

//-- Blink LED Declarations and Function -------------------------------------
void run_rgb_blink(void) {
  digitalWrite(GLED, HIGH);
  delay(1000);
  digitalWrite(GLED, LOW);
  delay(1000);
  digitalWrite(RLED, HIGH);
  delay(1000);
  digitalWrite(RLED, LOW);
  delay(1000);
  digitalWrite(BLED, HIGH);
  delay(1000);
  digitalWrite(BLED, LOW);
  delay(1000);
}

//-- Light LED By Key Declarations and Function ------------------------------
void init_key_light(void) {
  pinMode(GLED, OUTPUT);
  pinMode(KEYIN, INPUT);
  digitalWrite(GLED, LOW);
}
void run_key_light(void) {
  if (digitalRead(KEYIN) == 0)
    digitalWrite(GLED, HIGH);
  else
    digitalWrite(GLED, LOW);
  delay(100);
}


//-- Scan the Wifi Router ----------------------------------------------------
void get_wifi_scan(void) {
  //WiFi.mode(WIFI_STA);  // Set WiFi to station mode
  //WiFi.disconnect();  // Disconnect from an AP if it was previously connected

  String  ssid;
  int32_t rssi;
  uint8_t encryptionType;
  uint8_t* bssid;
  int32_t channel;
  bool hidden;
  int scanResult;

  Serial.println(F("Starting WiFi scan..."));
  scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

  if (scanResult == 0) { Serial.println(F("No networks found")); }
  else if (scanResult > 0) {
    Serial.printf(PSTR("%d networks found:\n"), scanResult);

    // Print unsorted scan results
    for (int8_t i = 0; i < scanResult; i++) {
      WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
      Serial.printf(PSTR("  %02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %s\n"),
        i, channel,
        bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5], rssi,
        (encryptionType == ENC_TYPE_NONE) ? ' ' : '*',
        hidden ? 'H' : 'V',
        ssid.c_str());
      delay(0);
    }
  }
  else {
    Serial.printf(PSTR("WiFi scan error %d"), scanResult);
  }
}


//-- init_wifi_link ------------------------------------------------------
void init_wifi_link(void) {
  Serial.print("Connect to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);    // Connect to WiFi network

  // 持續等待並連接到指定的 WiFi SSID
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}

//-- init_dht11_sense ----------------------------------------------------
void init_dht11_sense(void) {
  dht11_sensor.setup(5, DHTesp::DHT11); // Connect DHT11 to GPIO 5
  Serial.println("DHT11 Ready!");
  delay(2000);
}

//-- Upload to Thingspeak ----------------------------------------------------
void updateDHT11()
{
  // 設定 ESP8266 作為 Client 端
  WiFiClient client;
  if (!client.connect(HOST, PORT))
  {
    Serial.println("connection failed");
    return;
  }
  else
  {
    // 準備上傳到 ThingSpeak IoT Server 的資料
    // 已經預先設定好 ThingSpeak IoT Channel 的欄位
    // field1：溫度；field2：濕度
    String getStr = GET + "&field1=" + String((int)temperature) +
      "&field2=" + String((int)humidity) +
      " HTTP/1.1\r\n";;
    client.print(getStr);
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\r\n\r\n");
    delay(10);
    // 處理遠端伺服器回傳的訊息，程式碼可以寫在這裡！
    client.stop();
  }
}

//-- Upload to LineBot ----------------------------------------------------
void updateLine()
{
  String tempe = "溫度:" + String((int)(temperature - 32) * 5 / 9) + " C";
  String humid = "濕度:" + String((int)humidity) + "%";
  Serial.println(LINE.getVersion()); // Line Version
  LINE.setToken(LINE_TOKEN);
  LINE.notify("\n" + tempe + ";" + humid);
}
