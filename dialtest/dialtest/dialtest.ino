void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT_PULLUP);
  delay(2000);
  Serial.println("HELO");
}

int lastVal = -1;
int count = 0;
int buffer = 0;
const int nmax = 4;
int number[nmax] = {0,0,0,0};
int nptr = 0;

void loop() {
  int sensorVal = digitalRead(8);
  //Serial.println(sensorVal);
  if (sensorVal == lastVal) {
    count += 1;
    if (lastVal == 0 && count > 100 && buffer > 0) {
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
    if (lastVal == 0 && count > 10000 && nptr > 0) {
      nptr = 0;
      number[0] = 0;
      number[1] = 0;
      number[2] = 0;
      number[3] = 0;
      Serial.println("RESET!!!");
    }
  } else {
    if (lastVal == 1) {
      if (count > 10) {
        buffer += 1;
      }
    }
    if (lastVal == 0) {
      if (count > 100) {
        buffer = 0;
      }
    }
    count = 0;
    lastVal = sensorVal;
  }
  delay(1);
}
