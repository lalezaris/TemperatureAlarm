/*
  Temperature Alarm
  Pins:

  2  - Next Digit Button
  3  - Set Digit Button
  4  - Temperature Sensor Data
  7  - LCD RS
  8  - LCD E
  9  - LCD D4
  10 - LCD D5
  11 - LCD D6
  12 - LCD D7

*/

// include the library code:
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4
#define NEXT_DIGIT_PIN 3
#define SET_DIGIT_PIN 2

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
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


void setup() {
  cursorSetPos = 9;
  setTemp = 0;

  sensors.begin();

  // set up the LCD's number of columns and rows:
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

  // Open serial connection
  Serial.begin(9600);
}

void loop() {
  // Wait to refresh LCD after 5 seconds and a change in
  // temperature of .1 degree f
  while (waitToRefresh(5000) || tempIsSame(.2)) {
    setBlinkingCursor();
    checkForButtonInput();
  }
  writeTempToLCD();
}

/* Function: waitToRefresh
   Returns a boolean indicating if the program should wait

   waitInMillis: the amount of time to check in milliseconds

   return: true if the program should still wait - false otherwise
*/
boolean waitToRefresh(unsigned long waitInMillis) {
  unsigned long waitingTime = millis() - lastTime;
  if (waitingTime > waitInMillis) {
    lastTime = millis();
    return false;
  } else {
    return true;
  }
}

/* Function: tempIsSame
    Returns a boolean indicating if the previous temp reading and
    the current temp reading are within the threashold provided.

    sensitivity: the maximum difference in temperature

    return: true if the difference is greater than provided sensitivity
*/
boolean tempIsSame(float sensitivity) {
  sensors.requestTemperatures();
  curTemp = sensors.getTempFByIndex(0);
  
  float tempDiff = abs(curTemp - lastTemp);
  
  if (tempDiff < sensitivity) {
    return true;
  } else {
    lastTemp = curTemp;
    return false;
  }
}

void writeTempToLCD() {
  sensors.requestTemperatures();
  lcd.noBlink();
  lcd.setCursor(10, 0);
  curTemp = sensors.getTempFByIndex(0);
  dtostrf(curTemp, 5, 1, curTempChars);
  lcd.write(curTempChars, 5);
  lcd.setCursor(cursorSetPos, 1);
  lcd.blink();
}

void setBlinkingCursor() {
  if (cursorSetPos > 9 && cursorSetPos <= 14) {
    lcd.setCursor(cursorSetPos, 1);
    lcd.blink();
  } else {
    lcd.noBlink();
  }
}

void checkForButtonInput() {
  if (digitalRead(SET_DIGIT_PIN) == LOW) {
    if (previousSetButtonState == HIGH) {
      setDigitButtonAction();
    }
    previousSetButtonState = LOW;
  } else {
    previousSetButtonState = HIGH;
  }

  if (digitalRead(NEXT_DIGIT_PIN) == LOW) {
    if (previousNextButtonState == HIGH) {
      nextDigitButtonAction();
    }
    previousNextButtonState = LOW;
  } else {
    previousNextButtonState = HIGH;
  }
}
void setDigitButtonAction() {
  Serial.println("SET DIGIT BUTTON PRESSED");
  if (cursorSetPos == 10) {
    int curDigit = 0;
    char curChar = setTempChars[0];
    if (isdigit(curChar)) {
      curDigit = setTempChars[0] - '0';
    }
    if (curDigit >= 9) {
      setTemp -= 900;
    } else {
      setTemp += 100;
    }

    cursorSetPos = 10;
    lcd.setCursor(cursorSetPos, 1);

  } else if (cursorSetPos == 11) {
    int curDigit = 0;
    char curChar = setTempChars[1];
    if (isdigit(curChar)) {
      curDigit = setTempChars[1] - '0';
    }


    if (curDigit >= 9) {
      setTemp -= 90;
    } else {
      setTemp += 10;
    }
    cursorSetPos = 11;
    lcd.setCursor(cursorSetPos, 1);

  } else if (cursorSetPos == 12) {
    int curDigit = 0;
    char curChar = setTempChars[2];
    if (isdigit(curChar)) {
      curDigit = setTempChars[2] - '0';
    }


    if (curDigit >= 9) {
      setTemp -= 9;
    } else {
      setTemp += 1;
    }
    cursorSetPos = 12;
    lcd.setCursor(cursorSetPos, 1);

  } else if (cursorSetPos == 13) {
    // Decimal place
  } else if (cursorSetPos == 14) {
    int curDigit = 0;
    char curChar = setTempChars[4];
    if (isdigit(curChar)) {
      curDigit = setTempChars[4] - '0';
    }


    if (curDigit >= 9) {
      setTemp -= .9;
    } else {
      setTemp += .1;
    }
    cursorSetPos = 14;
    lcd.setCursor(cursorSetPos, 1);

  }
}

void nextDigitButtonAction() {
  // Serial.println("NEXT DIGIT BUTTON PRESSED");

  if (cursorSetPos <= 9) {
    cursorSetPos = 10;
  } else if (cursorSetPos < 14) {
    cursorSetPos ++;
  } else {
    cursorSetPos = 9;
  }
}

