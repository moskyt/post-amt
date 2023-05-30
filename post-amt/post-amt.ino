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
#include "die_post_360.h"

//--- pins and adresses

const int I2C_LCD = 0x27;
const int I2C_NEOKEY = 0x30;

const int PIN_LOAD_DATA = 2;
const int PIN_LOAD_CLK = 3;
const int PIN_BEEP = 4;
const int PIN_DIAL = 5;
const int PIN_LED_DATA = 7;
const int PIN_LED_CLK = 8;
const int PIN_RFID_RST = 9;
const int PIN_RFID_SS  = 10;
const int PIN_PRINTER_TX = 16;
const int PIN_PRINTER_RX = 17;
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
const int correct_code = 1234;

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

// global time at beginning of loop()
unsigned long now = 0;
// flag register of the neokey 
uint8_t buttons = 0;

// timers stuff -- taken from the library

// Select USING_16MHZ     == true for  16MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_8MHZ      == true for   8MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_250KHZ    == true for 250KHz to Timer TCBx => shorter timer, but better accuracy
// Not select for default 250KHz to Timer TCBx => longer timer,  but worse accuracy
#define USING_16MHZ     false
#define USING_8MHZ      false
#define USING_250KHZ    false

// Select the timers you're using, here ITimer1
#define USE_TIMER_0     false
#define USE_TIMER_1     true
#define USE_TIMER_2     false
#define USE_TIMER_3     false

#include "megaAVR_TimerInterrupt.h"

// --- dial stuff
// last value read
int dialLastVal = -1;
// timer/counter
int dialCount = 0;
// current number being read
int dialBuffer = 0;
// number of digits in code
const int dialNMax = 4;
// current code read
int dialNumber[dialNMax] = {0,0,0,0};
// current digit
int dialPtr = 0;

// what is available after init
// the idea here is that if neokey is not connected, any call results in reset
bool neokey_available = false;
bool rfid_available = false;

// what is currently read
// remember that 0 is a valid value for dial...
int dial_read = -1;
unsigned long card_read = -1;

// what is shown and when
int show_dial = -1;
unsigned long show_card = 0;
unsigned long int show_card_at = 0;
unsigned long int show_dial_at = 0;
const int show_delay = 10000; // actual ms

void loop() {

  now = millis();

  // read keyboard status
  if (neokey_available)
    buttons = neokey.read();

  // flag to show if any of the keyboard data was set
  // give me credit for this not being a global variable
  bool any_info_neo = false;
  if (neokey_available)
    for (uint16_t i=0; i<neokey.pixels.numPixels(); i++) 
      neokey.pixels.setPixelColor(i, 0x000000);

  if (rfid_available) read_rfid();
  display_scale_led();
  get_weight_from_scale();

  // waiting for card before sending?
  if (shipping_state == SHIPPING_CARD) 
    if ( (card_read == UID_VLK) || (card_read == UID_VORSILA) ) 
      print_shipment(card_read);

  // disable ux if printing the receipt
  if (neokey_available)
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

  // (de)queue other info for show
  if (show_dial > 0) {
    if (show_dial_at + show_delay < now) {
      show_dial = -1;
      show_dial_at = 0;
    }
  } 
  if (dial_read >= 0 && show_dial != dial_read) {
    show_dial = dial_read;
    show_dial_at = now;
  }
  if (show_card > 0) {
    if (show_card_at + show_delay < now) {
      show_card = 0;
      show_card_at = 0;
    }
  } 
  if (card_read > 0 && show_card != card_read) {
    show_card = card_read;
    show_card_at = now;
  }


  if (shipment_count > 0 || show_card_at > 0 || show_dial_at > 0) 
    lcd.backlight();

  if (show_card_at == now && (show_dial_at == 0 || show_card_at > show_dial_at)) {
    lcd.setCursor(0,2);
    lcd.print(F("KARTA:               "));
    lcd.setCursor(0,3);
    if (show_card == UID_VORSILA) 
      lcd.print(F("VORSILA             "));
    else if (show_card == UID_VLK) 
      lcd.print(F("VLK                 "));
    else
      lcd.print(show_card, HEX);
  } else if (show_dial_at == now) {
    lcd.setCursor(0,2);
    lcd.print(F("KOD ZAMKU:"));
    lcd.setCursor(0,3);
    if (show_dial < 1000) lcd.print(F("0"));
    if (show_dial < 100) lcd.print(F("0"));
    if (show_dial < 10) lcd.print(F("0"));
    lcd.print(show_dial);
  }
  
  // clear shipping info
  if (shipment_count == 0) lcd_clear_top();
  // clear card/dial info
  if ((show_card_at == 0) && (show_dial_at == 0)) lcd_clear_bottom();

  // shut off lcd if there is nothing going on
  if ( (shipment_count == 0) && (show_dial_at == 0) && (show_card_at == 0) ) {
    lcd.noBacklight();
    lcd.clear();
  }

  if (neokey_available) {
    // show pixels: either just use pale white or whatever was set by the ux
    if (!any_info_neo) 
      for (uint16_t i=0; i<neokey.pixels.numPixels(); i++) 
        neokey.pixels.setPixelColor(i, 0x333333);
    neokey.pixels.show();    
  }

  // unlock
  if (dial_read >= 0) {
    Serial.print(F("dial: "));
    Serial.println(dial_read);
    if (dial_read == correct_code) {
      dial_read = -1;
      Serial.println(F("Unlocking."));
      digitalWrite(PIN_LOCK, HIGH);
      delay(1000);
      digitalWrite(PIN_LOCK, LOW);
    } else {
      Serial.println(F("Code not correct."));
      beep1(500);
    }
    dial_read = -1;
  }

  // any delay is inherently bad, maybe we can refactor this to use timers. but...
  delay(50);
}
