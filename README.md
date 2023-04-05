# Hardware

## Komponenty

### mainboard
Arduino Nano Every (5V)

### zdroj - 5+12
MEANWELL RD 50A
https://www.gme.cz/v/1507485/mean-well-rd-50a-spinany-zdroj-pro-vestavbu-kryty
nebo mozna nejdriv zkusim rd35a
a stepdown z 12 na 8.5v pro tiskarnu
https://www.laskakit.cz/mikro-step-down-menic--nastavitelny

### bzucak
aktivni 5v bzucak

### ciselnik
https://aukro.cz/ciselnik-telefonu-zcela-funkcni-9x4-cm-7034546220

### LCD 20x4 s i2c
https://www.laskakit.cz/20x4-lcd-displej-2004-modry-i2c-prevodnik/

### vaha
https://www.laskakit.cz/guang-ce-yzc-131-vazici-senzor-3kg/
https://www.laskakit.cz/ad-prevodnik-modul-24-bit-2-kanaly-hx711/

### led displej pro vahu
4-digit 8-segment
https://www.laskakit.cz/hodinovy-displej-0-56--tm1637/

### RFID ctecka -- RC522 / 13.56 MHz
https://www.laskakit.cz/rfid-ctecka-s-vestavenou-antenou-mfrc-522-rc522/

### zamek
https://www.laskakit.cz/elektromagneticky-zamek-pro-dvere-12vdc-0-6a/
pres mosfet
https://www.laskakit.cz/pwm-mosfet-modul-lr7843--30vdc-161a/

### tiskarna
https://www.adafruit.com/product/597

### klavesnice
NeoKey
https://www.adafruit.com/product/4980

### osvetleni supliku
...

## Zapojeni

### Piny z Arduina

2+3 - vaha (data, clk)
4 - bzucak
7+8 - led (clk, data)
9+10 - rfid (ss, rst)
16+17 - tiskarna (rx, tx)
18+19 - I2C (neokey, LCD)
20 - zamek pres mosfet

### Pripojeni komponent na mainboard

R32-35 vaha[GND,DATA,CLK,VCC]
? bzucak[DATA,GND]
? rotacni ciselnik[DATA,GND]
R38-41 led[VCC,GND,?,?]
L23-20 lcd[GND,VCC,SDA,SCL]
L36-34 tiskarna[?,?,GND]
L45-38 rfid[3v3,?,GND,?,MISO,?,SCK,SS]
L30-27 neokey[VCC,GND,SCL,SDA]

zamek[2] -- do svorkovnice na mosfetu

5v napajeni -- do svorkovnice na mainboardu
12v napajeni -- do wago svorek

### 12V komponenty
+ mosfet pro zamek
+ sestava osvetleni supliku
+ napajeni pro tiskarnu

# Funkce

## Elektronika

### Vaha

po zatizeni se ukaze hmotnost
pokud neprobiha odesilani zasilek, lze tlacitkem 4 vynulovat taru

### Odesilani zasilek

po navazeni vybiram tlacitkem 1/2 balik nebo dopis
na LCD se mi ukaze pridana zasilka a pocet zasilek
tlacitkem 4 vsechno vyresetuju
tlacitkem 3 zahajim odesilani
po prilozeni karty se vytiskne listecek

### Suplik

po vytoceni trojmistneho kodu se otevre
po spatne cislici nebo prodleve se vyresetuje (cervene svetlo, zapipa, zhasne)

zapadka: vzadu shora
! pojezdy
! magnety misto pruziny

osvetleni:
nahore 12v led pasek
zapojeni pres mosfet a reed switch (dole) -- uplne mimo arduino

## Playware

### Razitka

giyf poststempel schweiz
    - kulaty postovni curych
    - die post
    - priority
    - misto znamky helvetia

plus polstarek a barva

vse v boxiku do jedne z policek

### Nalepky a formulare

v policce

### suplik s penezma

mince 1
bankovka 5 - orla
bankovka 10 - vorsila
bankovka 20 - vlk

## Servisni features

microusb vyvedeny dozadu

napajeni pres iec konektor

