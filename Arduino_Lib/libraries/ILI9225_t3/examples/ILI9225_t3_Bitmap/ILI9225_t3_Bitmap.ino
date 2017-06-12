/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include "SPI.h"
#include "ILI9225_t3.h"

static const unsigned char PROGMEM adafruit_icon[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

// For the Adafruit shield, these are the default.
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_MOSI 11  // MOSI
#define TFT_MISO 12  // MISO
#define TFT_CLK 13  // SCK
#define TFT_LED 6   // 255 if wired to +5V directly

#define ADAFRUIT_LOGO_HEIGHT 16
#define ADAFRUIT_LOGO_WIDTH  16

uint16_t cCount = 0;
// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9225_t3 tft = ILI9225_t3(TFT_CS, TFT_RS, TFT_RST, TFT_MOSI, TFT_CLK, TFT_MISO, TFT_LED);

void setup() {
  tft.begin();
  tft.fillScreen(COLOR_BLACK);
}


void loop(void) {
    for(int k = 0; k<tft.height(); k+=20)
    {
      for(int j = 0; j<tft.width(); j+=20)
      {
      if(cCount>=0xFFFF)
      cCount = 0;
      else
      tft.drawBitmap(j, k, adafruit_icon, 16, 16, cCount);
      cCount = random(0xFFFF);
      }
    }
}

