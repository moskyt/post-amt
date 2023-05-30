void lcd_clear_top() {
  lcd.setCursor(0,0);
  lcd.print(F("                    "));
  lcd.setCursor(0,1);
  lcd.print(F("                    "));
}

void lcd_clear_bottom() {
  lcd.setCursor(0,2);
  lcd.print(F("                    "));
  lcd.setCursor(0,3);
  lcd.print(F("                    "));
}

void lcd_print_shipments_nom() {
  lcd.print(shipment_count);
  if (shipment_count == 1)
    lcd.print(F(" zasilka"));
  else if (shipment_count <= 4)
    lcd.print(F(" zasilky"));
  else
    lcd.print(F(" zasilek"));
}


void lcd_print_shipments_acc() {
  lcd.print(shipment_count);
  if (shipment_count == 1)
    lcd.print(F(" zasilku"));
  else if (shipment_count <= 4)
    lcd.print(F(" zasilky"));
  else
    lcd.print(F(" zasilek"));
}
