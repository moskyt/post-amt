void setup() {
  // ---
  Serial.begin(115200);
  while (! Serial) delay(10);
  Serial.println(F("PostAmt HELO\n"));  

  // --- lock
  pinMode(PIN_LOCK, OUTPUT);
  digitalWrite(PIN_LOCK, LOW);
  
  // --- dial
  pinMode(PIN_DIAL, INPUT_PULLUP);

  // ---
  Serial.println(F("Init LCD ... "));  
  int status = lcd.begin(20, 4);
  if(status) {
    Serial.println(F("FAILED."));
  } else {
    Serial.println(F("OK."));  
  }

  lcd.setCursor(0,0);
  lcd.print(F("PostAmt"));
  lcd.setCursor(0,1);
  lcd.print(F("======="));
  delay(2000);

  // ---
  lcd.setCursor(0,3);
  lcd.print(F("(klavesnice)        "));
  Serial.println(F("Init NeoKey ... "));  
  if (!neokey.begin(I2C_NEOKEY)) {
    Serial.println(F("FAILED."));
    neokey_available = false;
  } else {
    Serial.println(F("OK."));  
    neokey_available = true;
  }

  neokey.pixels.setPixelColor(0, 0xFF0000);
  neokey.pixels.setPixelColor(1, 0xFFFF00);
  neokey.pixels.setPixelColor(2, 0x00FF00);
  neokey.pixels.setPixelColor(3, 0x0000FF);
  neokey.pixels.show();
  delay(1000);

  // ---
  lcd.setCursor(0,3);
  lcd.print(F("(vaha)           "));
  Serial.println(F("Init LED ... "));  
  led.setBrightness(0x0f);
  Serial.println(F("OK."));  
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  led.setSegments(data);

  // ---
  Serial.println(F("Init load cell ... "));  
  loadcell.begin();
  loadcell.start(2000, true);
  if (loadcell.getTareTimeoutFlag()) {
    Serial.println(F("FAILED"));
  } else {
    loadcell.setCalFactor(load_cell_cal);
    Serial.println(F("OK."));  
  }

  // ---
  Serial.println(F("Init beeper..."));
  pinMode(PIN_BEEP, OUTPUT);
  digitalWrite(PIN_BEEP, LOW);
  Serial.println(F("OK."));  
  
  // ---
  lcd.setCursor(0,3);
  lcd.print(F("(ctecka karet)     "));
  Serial.println(F("Init RFID ... "));  
  /*
  I had to comment this out because it made it non worky.
  But I am not happy about it at all.

  pinMode(PIN_RFID_RST, OUTPUT);
  digitalWrite(PIN_RFID_RST, LOW);
  delay(500);
  digitalWrite(PIN_RFID_RST, HIGH);
  delay(500);
  */
  mfrc522.PCD_Init();
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
  if (mfrc522.PCD_PerformSelfTest()) {
    rfid_available = true;
    Serial.println(F("OK."));  
  } else {
    rfid_available = false;
    Serial.println(F("FAILED."));  
  }


  // ---
  lcd.setCursor(0,3);
  lcd.print(F("(tiskarna)     "));
  Serial.println(F("Init printer ... "));  

  printerSerial.begin(19200);
  printer.begin();        
  printer.setFont('B');
  printer.feed(1);
  printer.sleep();      // Tell printer to sleep

  delay(2000);

  // ---
  scale_state = SCALE_OFF;
  scale_weight = 0;

  shipping_state = SHIPPING_COLLECTION;
  shipment_count = 0;

  neokey.pixels.setPixelColor(0, 0x333333);
  neokey.pixels.setPixelColor(1, 0x333333);
  neokey.pixels.setPixelColor(2, 0x333333);
  neokey.pixels.setPixelColor(3, 0x333333);
  neokey.pixels.show();

  // --- dial timer
	ITimer1.init();
  if (ITimer1.attachInterruptInterval(10, dialHandler))
    Serial.println(F("Timer init OK."));
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

  lcd.setCursor(0,3);
  lcd.print(F("muzes uradovat!   "));
  Serial.println(F("Init completed."));  

  // ---
  delay(2000);
  lcd.clear();
  lcd.noBacklight();  

}
