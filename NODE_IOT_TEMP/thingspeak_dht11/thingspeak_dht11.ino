#include "DHTesp.h"
#include <ESP8266WiFi.h> 
DHTesp dht;

float temperature, humidity;

//*-- IoT Information
#define SSID    "Art. XR"
#define PASS    "abcdef12"
#define HOST    "api.thingspeak.com" // ThingSpeak IP Address: 184.106.153.149
#define PORT    80
// 使用 GET 傳送資料的格式
// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&filed2=[data 2]...;
String GET = "GET /update?key=A7FWFGRLU2P1MOK9";

void setup() {
    Serial.begin(115200);
    Serial.println( "ESP8266 Ready!" );
    // Connecting to a WiFi network
    Serial.print("Connect to ");
    Serial.println( SSID );
    WiFi.begin( SSID, PASS );

    // 持續等待並連接到指定的 WiFi SSID
    while( WiFi.status() != WL_CONNECTED )
    {
        delay(500);
        Serial.print( "." );
    }
    Serial.println( "" );

    Serial.println( "WiFi connected" );
    Serial.println( "IP address: " );
    Serial.println( WiFi.localIP() );
    Serial.println( "" );
    
    // DHT11
    dht.setup(5,DHTesp::DHT11); // Connect DHT sensor to GPIO 5
    Serial.println( "DHT11 Ready!" );
    delay(2000);
}

void loop() {
    humidity    = dht.getHumidity();
    temperature = dht.getTemperature();

    // 確認取回的溫溼度數據可用
    if( isnan( humidity ) || isnan( temperature ) ){
        Serial.println( "Failed to read form DHT11" );
        return;}
    else{
        // DHT11 溫度與濕度傳送      
        Serial.print( "Humidity: " );
        Serial.print( humidity );
        Serial.print( ", Temperature: " );
        Serial.println( temperature );
        updateDHT11();
    }
    // 每隔多久傳送一次資料
    delay( 20000 ); // 20 second
}

void updateDHT11()
{
    // 設定 ESP8266 作為 Client 端
    WiFiClient client;
    if( !client.connect( HOST, PORT ) )
    {
        Serial.println( "connection failed" );
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
        client.print( getStr );
        client.print( "Host: api.thingspeak.com\n" );
        client.print( "Connection: close\r\n\r\n" );
        
        delay(10);
        //
        // 處理遠端伺服器回傳的訊息，程式碼可以寫在這裡！
        //


        client.stop();
    }
}
