
void display_scale_led() {
  if (scale_state == SCALE_OFF) {
    led_blank(); 
  }
  if (scale_state == SCALE_ON) {
    if (scale_weight >= 0) {
      if (scale_weight > 9999) {
        led_dashes();
      } else {
        led_number(scale_weight);
      }
    } else {
      led_number(0);
    }
  }
}

void get_weight_from_scale() {
  // get weight from load cell
  static int last_printed_weight = scale_weight;
  static boolean new_loadcell_data_ready = false;
  static unsigned long last_loadcell_read = 0;
  static unsigned long last_positive_weight = 0;
  if (loadcell.update()) new_loadcell_data_ready = true;
  if (now - last_loadcell_read > 30000)
    scale_weight = 0;
  if (new_loadcell_data_ready && ((now - last_loadcell_read > 1000) || (now < last_loadcell_read)) ) {
    scale_weight = floor(loadcell.getData());
    if (abs(scale_weight - last_printed_weight) >= 1) {
      Serial.print("measured weight = ");
      Serial.println(scale_weight);
      last_printed_weight = scale_weight;
    }
  }

  // turn scale on or off
  if (scale_weight >= min_load) {
    scale_state = SCALE_ON;
    last_positive_weight = now;
  } else if (now - last_positive_weight > scale_zero_weight_timeout) {
    scale_state = SCALE_OFF;
    last_positive_weight = 0;    
  }
}

void handle_tare(bool &any_info_neo) {
  // if there is some weight applied, offer BTN_TARE to reset it if no parcel order is in progress
  bool set_tare = false;
  if (scale_weight < -1) {
    // negative load: we need to allow tare reset
    neokey.pixels.setPixelColor(BTN_TARE, 0x5500ff);
    any_info_neo = true;
    set_tare = true;
  } else if (scale_state == SCALE_OFF || scale_weight <= 0) {
    // no load - we don't care
  } else {
    // standard tare setting
    neokey.pixels.setPixelColor(BTN_TARE, 0x0000ff);    
    any_info_neo = true;
    set_tare = true;
  }
  // handle tare setting
  if (set_tare)
  if (buttons & (1<<BTN_TARE)) {
    Serial.println(F("Setting tare..."));
    beep();
    loadcell.tare();
    beep(2);
  }
}
