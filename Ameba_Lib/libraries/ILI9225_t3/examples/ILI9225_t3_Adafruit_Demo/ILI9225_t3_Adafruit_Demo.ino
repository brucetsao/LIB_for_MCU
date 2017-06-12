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

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9225_t3 tft = ILI9225_t3(TFT_CS, TFT_RS, TFT_RST, TFT_MOSI, TFT_CLK, TFT_MISO, TFT_LED);

void setup() {
  tft.begin();
  tft.fillScreen(COLOR_BLACK);
  tft.setTextColor(COLOR_YELLOW);
  tft.setTextSize(2);
  tft.println("Waiting for Arduino Serial Monitor...");

  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("ILI9225 Test!"); 

  Serial.println(F("Benchmark                Time (microseconds)"));

  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  delay(200);

  Serial.print(F("Text                     "));
  Serial.println(testText());
  delay(600);

  Serial.print(F("Lines                    "));
  Serial.println(testLines(COLOR_CYAN));
  delay(200);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(COLOR_RED, COLOR_BLUE));
  delay(200);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(COLOR_GREEN));
  delay(200);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(COLOR_YELLOW, COLOR_MAGENTA));
  delay(200);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, COLOR_MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, COLOR_WHITE));
  delay(200);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
  delay(200);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  delay(200);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  delay(200);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  delay(200);

  Serial.println(F("Done!"));
  Serial.println();

}

void loop(void) {
  for(uint8_t rotation=0; rotation<4; rotation++)
  {
  tft.setRotation(rotation);
  uint32_t startMicros = micros();
  testFillScreen();
  testText();
  testLines(COLOR_CYAN);
  testFastLines(COLOR_RED, COLOR_BLUE);
  testRects(COLOR_GREEN);
  testFilledRects(COLOR_YELLOW, COLOR_MAGENTA);
  testFilledCircles(10, COLOR_MAGENTA);
  testCircles(10, COLOR_WHITE);
  testTriangles();
  testFilledTriangles();
  testRoundRects();
  testFilledRoundRects();
  uint32_t endMicros = micros();
  Serial.print("Rotation ");
  Serial.print(rotation);
  Serial.print(" Total Time = ");
  Serial.println(endMicros - startMicros);
  }
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(COLOR_BLACK);
  tft.fillScreen(COLOR_RED);
  tft.fillScreen(COLOR_GREEN);
  tft.fillScreen(COLOR_BLUE);
  tft.fillScreen(COLOR_BLACK);
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(COLOR_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(COLOR_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(COLOR_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(COLOR_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(COLOR_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(COLOR_BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(COLOR_BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(COLOR_BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(COLOR_BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(COLOR_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(COLOR_BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(COLOR_BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(COLOR_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(COLOR_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(COLOR_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(COLOR_BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(COLOR_BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
  }

  return micros() - start;
}
