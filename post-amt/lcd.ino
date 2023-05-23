
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
