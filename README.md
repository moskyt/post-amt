# POSTAMT

Darek pro Vlka a Vorsilu k narozeninam 2023.

Obecne: posledni tyden provazela hardwarova dramata a tak je SW mirne nedotazeny. Urceno k libovolnym upravam chovani i implementace podle potreby a aktualnich napadu.

## Hardware

### Komponenty

#### mainboard

Arduino Nano Every (5V)

nevim proc a nemel jsem prostor na dalsi zkoumani, ale pokud je pripojeny zdroj do site, tak se mi nedari pripojit pres usb.

#### zdroj - 5+12

MEANWELL RD 35A
https://www.gme.cz/v/1506286/mean-well-rd-35a-spinany-zdroj-pro-vestavbu-kryty

tepdown z 12 na 8.5v pro tiskarnu
https://www.laskakit.cz/mikro-step-down-menic--nastavitelny

#### bzucak

aktivni 5v bzucak

#### ciselnik

vykuchany rotacni ciselnik z telefonu
https://aukro.cz/ciselnik-telefonu-zcela-funkcni-9x4-cm-7034546220

#### LCD 

znakovy LCD 20x4 s i2c
https://www.laskakit.cz/20x4-lcd-displej-2004-modry-i2c-prevodnik/

#### vaha

trikilogramovy vahovy senzor
https://www.laskakit.cz/guang-ce-yzc-131-vazici-senzor-3kg/
https://www.laskakit.cz/ad-prevodnik-modul-24-bit-2-kanaly-hx711/

#### led displej pro vahu

4-digit 8-segment
https://www.laskakit.cz/hodinovy-displej-0-56--tm1637/

#### RFID ctecka -- RC522 / 13.56 MHz

https://www.laskakit.cz/rfid-ctecka-s-vestavenou-antenou-mfrc-522-rc522/

#### zamek

https://www.laskakit.cz/elektromagneticky-zamek-pro-dvere-12vdc-0-6a/

pres mosfet
https://www.laskakit.cz/pwm-mosfet-modul-lr7843--30vdc-161a/

#### tiskarna

https://www.adafruit.com/product/597

#### klavesnice

NeoKey
https://www.adafruit.com/product/4980

### Zapojeni

#### Piny z Arduina

2+3 - vaha (data, clk)
4 - bzucak
5 - rotacni ciselnik
7+8 - led (clk, data)
9+10 - rfid (ss, rst)
16+17 - tiskarna (rx, tx)
18+19 - I2C (neokey, LCD)
20 - zamek pres mosfet

#### Pripojeni komponent na mainboard

R32-35 vaha[GND,DATA,CLK,VCC]
? bzucak[+,GND]
R27-28 rotacni ciselnik[*,*]
R38-41 led[VCC,GND,DATA,CLK]
L23-20 lcd[GND,VCC,SDA,SCL]
L36-34 tiskarna[?,?,GND]
L45-38 rfid[3v3,?,GND,?,MISO,?,SCK,SS]
L30-27 neokey[VCC,GND,SCL,SDA]

zamek -- do svorkovnice na mosfetu

5v napajeni -- do svorkovnice na mainboardu
12v napajeni -- do wago svorek

#### 12V komponenty

mosfet pro zamek

napajeni pro tiskarnu (stepdown na 8.5VÎ)

## Funkce

### Vaha

po zatizeni se ukaze hmotnost

pokud neprobiha odesilani zasilek, lze tlacitkem vynulovat taru

### Odesilani zasilek

po navazeni vybiram tlacitkem balik nebo dopis

na LCD se mi ukaze pridana zasilka a pocet zasilek

tlacitkem vsechno vyresetuju

tlacitkem zahajim odesilani

po prilozeni karty se vytiskne listecek

### Suplik

po vytoceni kodu 1234 se otevre

zapadka: vzadu shora

automaticke otevirani na odpuzujici se magnety vedlo k tomu, ze se zapadka nedokazala otevrit. nestihnul jsem vyzkouset ruzne alternativy, ale mozna by to spravne slaba pruzina nebo spravna dvojice magnetu vyresila

## Playware

nejaky bizar z aukra

par osvojenych formularu z ceske posty

pokud mi bude sihlpost milostiv, tak https://shop.post.ch/shop/ui/de/geschenke/spielwaren/kinderpost-zubehor/p/507512