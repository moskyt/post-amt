
void lcd_print_shipments_nom() {
  lcd.print(shipment_count);
  if (shipment_count == 1)
    lcd.print(" zasilka");
  else if (shipment_count <= 4)
    lcd.print(" zasilky");
  else
    lcd.print(" zasilek");
}


void lcd_print_shipments_acc() {
  lcd.print(shipment_count);
  if (shipment_count == 1)
    lcd.print(" zasilku");
  else if (shipment_count <= 4)
    lcd.print(" zasilky");
  else
    lcd.print(" zasilek");
}
