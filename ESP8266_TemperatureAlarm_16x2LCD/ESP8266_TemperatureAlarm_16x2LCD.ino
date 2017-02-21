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
#include <LiquidCrystal.h>

//#include <Adafruit_SSD1306.h>
//#include <SPI.h>
//#include <Wire.h>

//#include <Adafruit_ssd1306syp.h>
#define SDA_PIN 5
#define SCL_PIN 4
//Adafruit_ssd1306syp display(SDA_PIN,SCL_PIN);

#define ONE_WIRE_BUS 4
#define NEXT_DIGIT_PIN 0
#define SET_DIGIT_PIN 2
#define RS_PIN 14
#define E_PIN 12
#define D4_PIN 13
#define D5_PIN 15
#define D6_PIN 3
#define D7_PIN 1

const char* ssid     = "NETGEAR-Guest";
const char* password = "KiraKoda2";

const char* host = "maker.ifttt.com";
const String url = "/trigger/temp-alarm/with/key/dWz0KZtTiIGoS3MXdtXk8q?";

int value = 0;

LiquidCrystal lcd(RS_PIN, E_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);
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
  lcd.begin(16, 2);
  
  // Print message to the LCD.
  lcd.print("Cur. Temp:     F");
  lcd.setCursor(0, 1);
  lcd.print("Alarm Set:     F");
  lcd.setCursor(10, 1);
  dtostrf(setTemp, 5, 1, setTempChars);
  lcd.write(setTempChars, 5);

  // Setup the buttons
  pinMode(SET_DIGIT_PIN, INPUT_PULLUP);
  pinMode(NEXT_DIGIT_PIN, INPUT_PULLUP);

//  writeTempToLCD();

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
