
void beep() {
  digitalWrite(PIN_BEEP, HIGH);
  delay(100);
  digitalWrite(PIN_BEEP, LOW);
}

void beep1(int m) {
  digitalWrite(PIN_BEEP, HIGH);
  delay(m);
  digitalWrite(PIN_BEEP, LOW);
}

void beep(int n) {
  for (int i = 0; i < n; ++i) {
    digitalWrite(PIN_BEEP, HIGH);
    delay(100);
    digitalWrite(PIN_BEEP, LOW);
    delay(100);
  }
}

void long_beep() {
  digitalWrite(PIN_BEEP, HIGH);
  delay(400);
  digitalWrite(PIN_BEEP, LOW);
  delay(100);
  digitalWrite(PIN_BEEP, HIGH);
  delay(400);
  digitalWrite(PIN_BEEP, LOW);
}
