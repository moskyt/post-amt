// timers stuff

// Select USING_16MHZ     == true for  16MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_8MHZ      == true for   8MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_250KHZ    == true for 250KHz to Timer TCBx => shorter timer, but better accuracy
// Not select for default 250KHz to Timer TCBx => longer timer,  but worse accuracy#define USING_16MHZ     false
#define USING_8MHZ      false
#define USING_250KHZ    false

// Select the timers you're using, here ITimer1
#define USE_TIMER_0     false
#define USE_TIMER_1     true
#define USE_TIMER_2     false
#define USE_TIMER_3     false

#include "megaAVR_TimerInterrupt.h"


const int pin=5;

int lastVal = -1;
int count = 0;
int buffer = 0;
const int nmax = 4;
int number[nmax] = {0,0,0,0};
int nptr = 0;

void handler() {

  int sensorVal = digitalRead(pin);
  //Serial.println(sensorVal);
  if (sensorVal == lastVal) {
    count += 1;
    if (lastVal == 0 && count > 5 && buffer > 0) {
      number[nptr] = buffer;
      nptr++;
      if (nptr < 4) {
        Serial.print(number[0]);
        Serial.print("-");
        if (nptr > 1) {
          Serial.print(number[1]);
          Serial.print("-");
        }
        if (nptr > 2) {
          Serial.print(number[2]);
          Serial.print("-");
        }
        Serial.println();
      }
      if (nptr == 4) {
        Serial.println("DIAL");
        Serial.print(number[0]);
        Serial.print("-");
        Serial.print(number[1]);
        Serial.print("-");
        Serial.print(number[2]);
        Serial.print("-");
        Serial.print(number[3]);
        Serial.println();

        nptr = 0;
        number[0] = 0;
        number[1] = 0;
        number[2] = 0;
        number[3] = 0;
      }

      buffer = 0;
    }
    if (lastVal == 0 && count > 1000 && nptr > 0) {
      nptr = 0;
      number[0] = 0;
      number[1] = 0;
      number[2] = 0;
      number[3] = 0;
      Serial.println("RESET!!!");
    }
  } else {
    if (lastVal == 1) {
      if (count > 1) {
        buffer += 1;
      }
    }
    if (lastVal == 0) {
      if (count > 10) {
        buffer = 0;
      }
    }
    count = 0;
    lastVal = sensorVal;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pin, INPUT_PULLUP);
  delay(2000);
  Serial.println("HELO");

	ITimer1.init();

 if (ITimer1.attachInterruptInterval(10, handler))
    Serial.println("Starting  ITimer1 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer1. Select another freq. or timer");
}


void loop() {}

