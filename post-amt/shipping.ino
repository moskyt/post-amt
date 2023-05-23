void reset_or_send(bool &any_info_neo) {
  // reset and send functions
  if (shipping_state == SHIPPING_COLLECTION) {
  
    neokey.pixels.setPixelColor(BTN_RESET, 0xff0000);    
    neokey.pixels.setPixelColor(BTN_SEND,  0x00ff00);    
    
    any_info_neo = true;
    if (buttons & (1<<BTN_RESET)) {
      Serial.println(F("Clearing shipments..."));
      shipment_count = 0;
      long_beep();
    }
    if (buttons & (1<<BTN_SEND)) {
      beep(2);
      Serial.println(F("Sending shipments, present card!"));
      shipping_state = SHIPPING_CARD;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("Odesilam "));
      lcd_print_shipments_acc();
      lcd.setCursor(0,1);
      lcd.print(F("Priloz kartu!"));
    }
  } else
  if (shipping_state == SHIPPING_CARD) {
    neokey.pixels.setPixelColor(BTN_RESET, 0xff0000);    
    if (buttons & (1<<BTN_RESET)) {
      Serial.println(F("Aborting send and clearing shipments..."));
      shipment_count = 0;
      shipping_state = SHIPPING_COLLECTION;
      long_beep();
    }
  }
}

void add_shipments(bool &any_info_neo) {
  // if there is a parcel, allow adding it to parcel list
 if (shipping_state == SHIPPING_COLLECTION && scale_state == SCALE_ON && scale_weight > min_load) {
    any_info_neo = true;
    neokey.pixels.setPixelColor(BTN_ADD_PARCEL, 0x00FFFF);
    neokey.pixels.setPixelColor(BTN_ADD_LETTER, 0x00FFFF);
    shipment_type_ add_type = SHIPMENT_TYPE_NONE;
    if (buttons & (1<<BTN_ADD_PARCEL)) {
      add_type = SHIPMENT_TYPE_PARCEL;
    }
    if (buttons & (1<<BTN_ADD_LETTER)) {
      add_type = SHIPMENT_TYPE_LETTER;
    }
    if (add_type != SHIPMENT_TYPE_NONE) {
      if (shipment_count < max_shipment_count) {
        beep();
        lcd.clear();
        lcd.backlight();
        // wait until there is no button pressed
        while ((buttons & (1<<BTN_ADD_PARCEL)) || (buttons & (1<<BTN_ADD_LETTER))) buttons = neokey.read();
        lcd.setCursor(0,0);
        if (add_type == SHIPMENT_TYPE_PARCEL)
          lcd.print(F("Balik"));
        else if (add_type == SHIPMENT_TYPE_LETTER)
          lcd.print(F("Dopis"));
        else
          lcd.print(F("Jina zasilka"));
        shipment_count++;
        lcd.setCursor(0,1);
        lcd.print(scale_weight);
        lcd.print(F(" g"));
        lcd.setCursor(0,3);
        lcd.print(F("Celkem "));
        lcd_print_shipments_nom();

        shipment_weight[shipment_count-1] = scale_weight;
        shipment_type[shipment_count-1] = add_type;
      } else {
        long_beep();
      }
    }
  }
}

void print_shipment(unsigned long uid) {
  beep(3);
  shipping_state = SHIPPING_PRINTING;
  lcd.clear(); 

  lcd.setCursor(0,0);
  lcd.print(F("Odesilam "));
  lcd_print_shipments_acc();
  lcd.setCursor(0,1);

  if ( (uid == UID_VLK) ) lcd.print(F("VLK"));
  if ( (uid == UID_VORSILA) ) lcd.print(F("VORSILA"));
  lcd.setCursor(0,2);
  lcd.setCursor(0,3);
  lcd.print(F("Tisknu stvrzenku..."));

  printer.wake();

  printer.printBitmap(die_post_360_width, die_post_360_height, die_post_360_data);
  printer.feed(1);

  printer.setFont('B');
  printer.setSize('L');
  printer.setLineHeight(40);
  printer.inverseOn();
  if ( (uid == UID_VLK) ) printer.println("VLK");
  if ( (uid == UID_VORSILA) ) printer.println("VORSILA");
  printer.inverseOff();
  printer.setSize('M');
  printer.feed(1);
  for (int i = 0; i < shipment_count; ++i) {
    if (shipment_type[i] == SHIPMENT_TYPE_PARCEL)
      printer.println("Balik");
    else if (shipment_type[i] == SHIPMENT_TYPE_LETTER)
      printer.println("Dopis");
    else
      printer.println("Jina zasilka");
    printer.print(shipment_weight[i]);
    printer.println(" g");
    printer.println("");
  }
  printer.feed(2);
  printer.sleep();    
  
  beep();
  lcd.clear();
  lcd.noBacklight();  

  shipping_state = SHIPPING_COLLECTION;
  shipment_count = 0;
  card_read = 0;
}
