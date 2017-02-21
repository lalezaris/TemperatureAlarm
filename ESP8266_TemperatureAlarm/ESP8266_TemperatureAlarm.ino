/*
  Temperature Alarm for ESP8266
  Pins:

  0  - Next Digit Button
  2  - Set Digit Button
  4  - OLED SCL Pin
  5  - OLED SDA Pin  
  14 - Temperature Sensor Data
 */

#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_ssd1306syp.h>
#define SDA_PIN 5
#define SCL_PIN 4
Adafruit_ssd1306syp display(SDA_PIN,SCL_PIN);

#define ONE_WIRE_BUS 14
#define NEXT_DIGIT_PIN 0
#define SET_DIGIT_PIN 2

const char* ssid     = "NETGEAR-Guest";
const char* password = "KiraKoda2";

const char* host = "maker.ifttt.com";
const String url = "/trigger/temp-alarm/with/key/dWz0KZtTiIGoS3MXdtXk8q?";

int value = 0;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float curTemp;
char curTempChars[6];
char setTempChars[6];
String curTempString;
float setTemp;
int cursorSetPos;
unsigned long lastTime = 0;
float lastTemp = 0;
int previousSetButtonState = HIGH;
int previousNextButtonState = HIGH;
float timeLastBeep = 0;

void setup() {
  Serial.begin(115200);
  cursorSetPos = 9;
  setTemp = 0;

  sensors.begin();

  // set up the display's number of columns and rows:
  display.initialize();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  // Print message to the display.
  display.println("Cur. Temp:");
  display.print("68.3");
  display.setTextSize(1);
  display.print("f");
  display.setTextSize(2);
  display.print(" 20.2");
  display.setTextSize(1);
  display.print("c");
  display.setTextSize(2);
  display.println("");
  display.println("Alarm Set:");
  display.println("32.0F 00.0C");
  display.update();
//  display.setCursor(10, 1);
//  dtostrf(setTemp, 5, 1, setTempChars);
//  display.write(setTempChars, 5);

  // Setup the buttons
  pinMode(SET_DIGIT_PIN, INPUT_PULLUP);
  pinMode(NEXT_DIGIT_PIN, INPUT_PULLUP);

  testVerticalPixels();
  // Open serial connection
}

void testVerticalPixels(){
//  for(uint16_t y=0; y<64; y+=4){
//    for(uint16_t x=100; x<128; x++){
//    display.drawPixel(x, y, WHITE);
//    }
//    delay(500);
//    display.update();
//  }

  for(uint16_t x=0; x<128; x++){
    display.drawPixel(x, 30, WHITE);
    display.drawPixel(x, 62, WHITE);
  } 
  display.update();
}

void loop() {
  
}

void sendAlarmServerRequest(String value1, String value2){
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request

  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + "value1=" + value1 + "&value2=" + value2 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  Serial.println(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  }
