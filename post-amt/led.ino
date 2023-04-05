
void led_number(int w) {  
  uint8_t data[] = { 0x00, 0x00, 0x00, 0x00 };
  data[0] = led.encodeDigit((w / 1000) % 10);
  data[1] = led.encodeDigit((w / 100 ) % 10);
  data[2] = led.encodeDigit((w / 10  ) % 10);
  data[3] = led.encodeDigit((w       ) % 10);
  led.setSegments(data);
}

void led_blank() {
  uint8_t data[] = { 0x00, 0x00, 0x00, 0x00 };
  led.setSegments(data);  
}

void led_dashes() {
  uint8_t data[] = { SEG_G, SEG_G, SEG_G, SEG_G };
  led.setSegments(data);  
}
