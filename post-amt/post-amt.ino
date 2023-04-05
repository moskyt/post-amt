#include "Adafruit_Thermal.h"
#include "Adafruit_NeoKey_1x4.h"
#include "seesaw_neopixel.h"
#include "SoftwareSerial.h"
#include <Wire.h>
#include <hd44780.h>                      
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <HX711_ADC.h>
#include <TM1637Display.h>

//--- pins and adresses
// available_pins: 5,6

const int I2C_LCD = 0x27;
const int I2C_NEOKEY = 0x30;

const int PIN_BEEP = 4;
const int PIN_RFID_RST = 9;
const int PIN_RFID_SS  = 10;
const int PIN_LOAD_DATA = 2;
const int PIN_LOAD_CLK = 3;
const int PIN_LED_DATA = 7;
const int PIN_LED_CLK = 8;
const int PIN_PRINTER_RX = 17;
const int PIN_PRINTER_TX = 16;
const int PIN_LOCK = 20;
const int PIN_ROTARY = 21;

//--- RFID card ids
const unsigned long int UID_VLK     = 0x9763D21E;
const unsigned long int UID_VORSILA = 0x5B693E3A;

//--- neokey functional assignment
const int BTN_ADD_PARCEL = 0;
const int BTN_ADD_LETTER = 1;
const int BTN_SEND = 2;
const int BTN_RESET = 3;
const int BTN_TARE = 3;

//--- configuration
const float load_cell_cal = 733.0; // this should be acquired from the calibration script; it is available as an example in the HX711_ADC library
const int min_load = 2;
const int scale_zero_weight_timeout = 5000;

//--- hw components

// RFID reader
MFRC522DriverPinSimple ss_pin(PIN_RFID_SS);
SPIClass &spiClass = SPI;
const SPISettings spiSettings = SPISettings(SPI_CLOCK_DIV4, MSBFIRST, SPI_MODE0); 
MFRC522DriverSPI driver{ss_pin, spiClass, spiSettings};
MFRC522 mfrc522{driver};

// LCD display
hd44780_I2Cexp lcd(I2C_LCD);

// LED display
TM1637Display led(PIN_LED_CLK, PIN_LED_DATA);

// neokey 4-button leopard
Adafruit_NeoKey_1x4 neokey;

// 3kg load cell with HX711
HX711_ADC loadcell(PIN_LOAD_DATA, PIN_LOAD_CLK);

//
SoftwareSerial printerSerial(PIN_PRINTER_RX, PIN_PRINTER_TX);
Adafruit_Thermal printer(&printerSerial);

//--- state management

enum shipment_type_ {SHIPMENT_TYPE_NONE, SHIPMENT_TYPE_PARCEL, SHIPMENT_TYPE_LETTER};

enum scale_state_ {SCALE_OFF, SCALE_ON} scale_state;

enum shipping_state_ {SHIPPING_COLLECTION, SHIPPING_CARD, SHIPPING_PRINTING} shipping_state;

int scale_weight = 0;
int shipment_count = 0;
const int max_shipment_count = 5;
int shipment_weight[max_shipment_count];
shipment_type_ shipment_type[max_shipment_count];

// current rfid card id
unsigned long rfid_uid = 0;
// global time at beginning of loop()
unsigned long now = 0;
// flag register of the neokey 
uint8_t buttons = 0;

void loop() {
  now = millis();
  buttons = neokey.read();

  // flag to show if any of the ke
  // give me credit for this not being a global variable
  bool any_info_neo = false;
  for (uint16_t i=0; i<neokey.pixels.numPixels(); i++) 
    neokey.pixels.setPixelColor(i, 0x000000);

  read_rfid();
  display_scale_led();
  get_weight_from_scale();

  // waiting for card before sending?
  if (shipping_state == SHIPPING_CARD) 
    if ( (rfid_uid == UID_VLK) || (rfid_uid == UID_VORSILA) ) 
      print_shipment(rfid_uid);

  // disable ux if printing the receipt
  if (shipping_state == SHIPPING_PRINTING) {    
    any_info_neo = true;
    for (uint16_t i=0; i<neokey.pixels.numPixels(); i++) 
      neokey.pixels.setPixelColor(i, 0xFF8888);    
  }

  if (shipment_count == 0)
    // allow tare setting if no shipping is in progress
    handle_tare(any_info_neo);
  else
    // handle finalization of shipping process (reset or proceed with sending)
    reset_or_send(any_info_neo);  
  // process adding a new item
  add_shipments(any_info_neo);

  // shut off lcd (rethink this...)
  if (shipment_count == 0) {
    lcd.clear();
    lcd.noBacklight();
  }
  
  // show pixels: either just use pale white or whatever was set by the ux
  if (!any_info_neo) 
    for (uint16_t i=0; i<neokey.pixels.numPixels(); i++) 
      neokey.pixels.setPixelColor(i, 0x333333);
  neokey.pixels.show();    

  // any delay is inherently bad, maybe we can refactor this to use timers. but...
  delay(50);
}
