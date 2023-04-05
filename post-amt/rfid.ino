void read_rfid() {
  static unsigned long last_rfid_read = 0;
  if ( mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() && ((now - last_rfid_read > 2000) || (now < last_rfid_read)) ) {
    last_rfid_read = now;
    
    unsigned long uid = 0;
    uid += (long)mfrc522.uid.uidByte[0] << 24;
    uid += (long)mfrc522.uid.uidByte[1] << 16;
    uid += (long)mfrc522.uid.uidByte[2] << 8;
    uid += (long)mfrc522.uid.uidByte[3];
    Serial.println(uid,HEX); 
    rfid_uid = uid;
    if (shipping_state != SHIPPING_CARD) 
      beep();
  }
  if (now - last_rfid_read > 1000) {
    rfid_uid = 0;
  }
}