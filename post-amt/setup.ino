void setup() {
  // ---
  Serial.begin(115200);
  while (! Serial) delay(10);
  Serial.println("PostAmt HELO\n");  

  // ---
  Serial.println("Init LCD ... ");  
  int status = lcd.begin(20, 4);
  if(status) {
    Serial.println("FAILED.");
  }
  Serial.println("OK.");  

  lcd.setCursor(0,0);
  lcd.print("PostAmt");
  lcd.setCursor(0,1);
  lcd.print("=======");
  delay(2000);

  // ---
  lcd.setCursor(0,3);
  lcd.print("(klavesnice)        ");
  Serial.println("Init NeoKey ... ");  
  if (!neokey.begin(I2C_NEOKEY)) {
    Serial.println("FAILED.");
  }
  Serial.println("OK.");  

  neokey.pixels.setPixelColor(0, 0xFF0000);
  neokey.pixels.setPixelColor(1, 0xFFFF00);
  neokey.pixels.setPixelColor(2, 0x00FF00);
  neokey.pixels.setPixelColor(3, 0x0000FF);
  neokey.pixels.show();
  delay(1000);

  // ---
  lcd.setCursor(0,3);
  lcd.print("(vaha)           ");
  Serial.println("Init LED ... ");  
  led.setBrightness(0x0f);
  Serial.println("OK.");  
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  led.setSegments(data);

  // ---
  Serial.println("Init load cell ... ");  
  loadcell.begin();
  loadcell.start(2000, true);
  if (loadcell.getTareTimeoutFlag()) {
    Serial.println("FAILED");
    //while (1);
  } else {
    loadcell.setCalFactor(load_cell_cal);
  }
  Serial.println("OK.");  

  // ---
  Serial.println(F("Init beeper..."));
  pinMode(PIN_BEEP, OUTPUT);
  digitalWrite(PIN_BEEP, LOW);
  Serial.println("OK.");  

  // ---
  lcd.setCursor(0,3);
  lcd.print("(ctecka karet)     ");
  Serial.println("Init RFID ... ");  
  pinMode(PIN_RFID_RST, OUTPUT);
  digitalWrite(PIN_RFID_RST, LOW);
  delay(500);
  digitalWrite(PIN_RFID_RST, HIGH);
  delay(500);
  mfrc522.PCD_Init();
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
  Serial.println("OK.");  


  // ---
  lcd.setCursor(0,3);
  lcd.print("(tiskarna)     ");
  Serial.println("Init printer ... ");  

  printerSerial.begin(19200);
  printer.begin();        

  printer.setFont('B');
  printer.println("PostAmt");
  printer.println("-------");
  printer.feed(5);

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

  lcd.setCursor(0,3);
  lcd.print("muzes uradovat!   ");
  Serial.println("Init completed.");  

  // ---
  delay(2000);
  lcd.clear();
  lcd.noBacklight();  
}
