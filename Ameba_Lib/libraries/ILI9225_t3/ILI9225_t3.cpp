// https://github.com/PaulStoffregen/ILI9341_t3
// http://forum.pjrc.com/threads/26305-Highly-optimized-ILI9341-(320x240-TFT-color-display)-library

/***************************************************
  This is our library for the Adafruit ILI9341 Breakout and Shield
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

#include "ILI9225_t3.h"
#include <SPI.h>

// Teensy 3.1 can only generate 30 MHz SPI when running at 120 MHz (overclock)
// At all other speeds, SPI.beginTransaction() will use the fastest available clock
#define SPICLOCK 30000000

#define WIDTH  (ILI9225_TFTWIDTH-1)
#define HEIGHT (ILI9225_TFTHEIGHT-1)

// Constructor when using hardware SPI.  Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
ILI9225_t3::ILI9225_t3(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t sclk, uint8_t miso, uint8_t led)
{
_cs   = cs;
_dc   = dc;
_rst  = rst;
_mosi = mosi;
_sclk = sclk;
_miso = miso;
_led  = led;
_width    = WIDTH;
_height   = HEIGHT;
rotation  = 0;
cursor_y  = cursor_x    = 0;
textsize  = 1;
textcolor = textbgcolor = 0xFFFF;
wrap      = true;
}

void ILI9225_t3::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
writeRegister(endH, x1);
writeRegister(startH, x0);
writeRegister(endV, y1);
writeRegister(startV, y0);
writeRegister(ramAddrOne, x0);
writeRegister(ramAddrTwo, y0);
writecommand_cont(ILI9225_GRAM_DATA_REG);
}

void ILI9225_t3::pushColor(uint16_t color)
{
SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
writedata16_last(color);
SPI.endTransaction();
}

void ILI9225_t3::drawPixel(uint16_t x1, uint16_t y1, uint16_t color)
{
  if((x1 < 0) || (x1 > _width) || (y1 < 0) || (y1 > _height)) return;
setAddrWindow(x1, y1, x1, y1);
writedata16_last(color);
SPI.endTransaction();
}

void ILI9225_t3::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
// Rudimentary clipping
  if((x > _width) || (y > _height)) return;
  if((y+h) > _height) h = _height-y;
setAddrWindow(x, y, x, y+h);
  while (h-- > 0)
  writedata16_cont(color);
writedata16_last(color);
SPI.endTransaction();
}

void ILI9225_t3::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
// Rudimentary clipping
  if((x > _width) || (y > _height)) return;
  if((x+w) > _width)  w = _width-x;
setAddrWindow(x, y, x+w, y);
  while (w-- > 0)
  writedata16_cont(color);
writedata16_last(color);
SPI.endTransaction();
}

void ILI9225_t3::fillScreen(uint16_t color)
{
fillRect(0, 0, _width, _height, color);
}

// fill a rectangle
void ILI9225_t3::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
// rudimentary clipping (drawChar w/big text requires this)
  if((x > _width) || (y > _height)) return;
  if((x + w) > _width)  w = _width - x;
  if((y + h) > _height) h = _height - y;

// TODO: this can result in a very long transaction time
// should break this into multiple transactions, even though
// it'll cost more overhead, so we don't stall other SPI libs

setAddrWindow(x, y, x+w, y+h);
  for(y=h;y>=0;y--)
  {
	for(x=w;x>=0;x--)
    writedata16_cont(color);
  writedata16_cont(color);
  }
writedata16_last(color);
SPI.endTransaction();
}

void ILI9225_t3::setRotation(uint8_t r, uint8_t i)
{
rotation = r % 4; // can't be higher than 3
i = i % 2; // can't be higher than 1
SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
writecommand_cont(ILI9225_DRIVER_OUTPUT_CTRL);

  if(!rotation)
  {
  writedata16_cont(i ? 0x001C:0x011C);
  writecommand_cont(ILI9225_ENTRY_MODE);
  writedata16_last(0x1030);
  SPI.endTransaction();
  startH = ILI9225_HORIZONTAL_WINDOW_ADDR2;
  endH = ILI9225_HORIZONTAL_WINDOW_ADDR1;
  startV = ILI9225_VERTICAL_WINDOW_ADDR2;
  endV = ILI9225_VERTICAL_WINDOW_ADDR1;
  ramAddrOne = ILI9225_RAM_ADDR_SET1;
  ramAddrTwo = ILI9225_RAM_ADDR_SET2;
  _width = WIDTH;
  _height = HEIGHT;
  }
  else if(rotation == 1)
  {
  writedata16_cont(i ? 0x021C:0x001C);
  writecommand_cont(ILI9225_ENTRY_MODE);
  writedata16_last(0x1038);
  SPI.endTransaction();
  startH = ILI9225_VERTICAL_WINDOW_ADDR2;
  endH = ILI9225_VERTICAL_WINDOW_ADDR1;
  startV = ILI9225_HORIZONTAL_WINDOW_ADDR2;
  endV = ILI9225_HORIZONTAL_WINDOW_ADDR1;
  ramAddrOne = ILI9225_RAM_ADDR_SET2;
  ramAddrTwo = ILI9225_RAM_ADDR_SET1;
  _width = HEIGHT;
  _height = WIDTH;
  }
  else if(rotation == 2)
  {
  writedata16_cont(i ? 0x031C:0x021C);
  writecommand_cont(ILI9225_ENTRY_MODE);
  writedata16_last(0x1030);
  SPI.endTransaction();
  startH = ILI9225_HORIZONTAL_WINDOW_ADDR2;
  endH = ILI9225_HORIZONTAL_WINDOW_ADDR1;
  startV = ILI9225_VERTICAL_WINDOW_ADDR2;
  endV = ILI9225_VERTICAL_WINDOW_ADDR1;
  ramAddrOne = ILI9225_RAM_ADDR_SET1;
  ramAddrTwo = ILI9225_RAM_ADDR_SET2;
  _width = WIDTH;
  _height = HEIGHT;
  }
  else if(rotation == 3)
  {
  writedata16_cont(i ? 0x011C:0x031C);
  writecommand_cont(ILI9225_ENTRY_MODE);
  writedata16_last(0x1038);
  SPI.endTransaction();
  startH = ILI9225_VERTICAL_WINDOW_ADDR2;
  endH = ILI9225_VERTICAL_WINDOW_ADDR1;
  startV = ILI9225_HORIZONTAL_WINDOW_ADDR2;
  endV = ILI9225_HORIZONTAL_WINDOW_ADDR1;
  ramAddrOne = ILI9225_RAM_ADDR_SET2;
  ramAddrTwo = ILI9225_RAM_ADDR_SET1;
  _width = HEIGHT;
  _height = WIDTH;
  }
}

void ILI9225_t3::invertDisplay(boolean i)
{
setRotation(rotation, i ? 1:0);
}

void ILI9225_t3::setDisplay(boolean flag) // New
{
SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
writecommand_cont(0x00ff);
writedata16_cont(0x0000);
writecommand_cont(ILI9225_POWER_CTRL1);
writedata16_cont(flag ? 0x0000 : 0x0003);
writecommand_cont(ILI9225_DISP_CTRL1);
writedata16_last(flag ? 0x1017 : 0x0000);
SPI.endTransaction();
delay(150);
}

void ILI9225_t3::setBacklight(uint8_t val) // New
{
  if(_led < 255) analogWrite(_led, val);
}


void ILI9225_t3::begin()
{
    // verify SPI pins are valid;
  if ((_mosi == 11 || _mosi == 7) && (_miso == 12 || _miso == 8) && (_sclk == 13 || _sclk == 14))
  {
  SPI.setMOSI(_mosi);
  SPI.setMISO(_miso);
  SPI.setSCK(_sclk);
  }
  else
  {return;} // not valid pins...
SPI.begin();
  if (SPI.pinIsChipSelect(_cs, _dc))
  {
  pcs_data = setCS(_cs);
  pcs_command = pcs_data | setCS(_dc);
  }
  else
  {
  pcs_data = 0;
  pcs_command = 0;
  return;
  }
  if (_rst < 255)
  {
  pinMode(_rst, OUTPUT);
  digitalWrite(_rst, HIGH);
  delay(5);
  digitalWrite(_rst, LOW);
  delay(20);
  digitalWrite(_rst, HIGH);
  delay(150);
  }
  if (_led < 255)
  {
  pinMode(_led, OUTPUT);
  analogWrite(_led, 255);
  }
SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
	// Power-on sequence
writeRegister(ILI9225_POWER_CTRL1, 0x0000); // Set SAP,DSTB,STB
writeRegister(ILI9225_POWER_CTRL2, 0x0000); // Set APON,PON,AON,VCI1EN,VC
writeRegister(ILI9225_POWER_CTRL3, 0x0000); // Set BT,DC1,DC2,DC3
writeRegister(ILI9225_POWER_CTRL4, 0x0000); // Set GVDD
writeRegister(ILI9225_POWER_CTRL5, 0x0000); // Set VCOMH/VCOML voltage
delay(40); 
// Power-on sequence
writeRegister(ILI9225_POWER_CTRL2, 0x0018); // Set APON,PON,AON,VCI1EN,VC
writeRegister(ILI9225_POWER_CTRL3, 0x6121); // Set BT,DC1,DC2,DC3
writeRegister(ILI9225_POWER_CTRL4, 0x006F); // Set GVDD   /*007F 0088 */
writeRegister(ILI9225_POWER_CTRL5, 0x495F); // Set VCOMH/VCOML voltage
writeRegister(ILI9225_POWER_CTRL1, 0x0F00); // Set SAP,DSTB,STB
delay(10);
writeRegister(ILI9225_POWER_CTRL2, 0x103B); // Set APON,PON,AON,VCI1EN,VC
delay(50);
writeRegister(ILI9225_DRIVER_OUTPUT_CTRL, 0x011C); // set the display line number and display direction
writeRegister(ILI9225_LCD_AC_DRIVING_CTRL, 0x0100); // set 1 line inversion
writeRegister(ILI9225_ENTRY_MODE, 0x01030); // set GRAM write direction and BGR=1.
writeRegister(ILI9225_DISP_CTRL1, 0x0000); // Display off
writeRegister(ILI9225_DISP_CTRL2, 0x0808); // set the back porch and front porch
writeRegister(ILI9225_FRAME_CYCLE_CTRL, 0x1100); // set the clocks number per line
writeRegister(ILI9225_INTERFACE_CTRL, 0x0000); // CPU interface
writeRegister(ILI9225_OSC_CTRL, 0x0D01); // Set Osc  /*0e01*/
writeRegister(ILI9225_VCI_RECYCLING, 0x0020); // Set VCI recycling
writeRegister(ILI9225_RAM_ADDR_SET1, 0x0000); // RAM Address
writeRegister(ILI9225_RAM_ADDR_SET2, 0x0000); // RAM Address
/* Set GRAM area */
writeRegister(ILI9225_GATE_SCAN_CTRL, 0x0000); 
writeRegister(ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB); 
writeRegister(ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000); 
writeRegister(ILI9225_VERTICAL_SCROLL_CTRL3, 0x0000); 
writeRegister(ILI9225_PARTIAL_DRIVING_POS1, 0x00DB); 
writeRegister(ILI9225_PARTIAL_DRIVING_POS2, 0x0000); 
writeRegister(ILI9225_HORIZONTAL_WINDOW_ADDR1, 0x00AF); 
writeRegister(ILI9225_HORIZONTAL_WINDOW_ADDR2, 0x0000); 
writeRegister(ILI9225_VERTICAL_WINDOW_ADDR1, 0x00DB); 
writeRegister(ILI9225_VERTICAL_WINDOW_ADDR2, 0x0000); 
/* Set GAMMA curve */
writeRegister(ILI9225_GAMMA_CTRL1, 0x0000); 
writeRegister(ILI9225_GAMMA_CTRL2, 0x0808); 
writeRegister(ILI9225_GAMMA_CTRL3, 0x080A); 
writeRegister(ILI9225_GAMMA_CTRL4, 0x000A); 
writeRegister(ILI9225_GAMMA_CTRL5, 0x0A08); 
writeRegister(ILI9225_GAMMA_CTRL6, 0x0808); 
writeRegister(ILI9225_GAMMA_CTRL7, 0x0000); 
writeRegister(ILI9225_GAMMA_CTRL8, 0x0A00); 
writeRegister(ILI9225_GAMMA_CTRL9, 0x0710); 
writeRegister(ILI9225_GAMMA_CTRL10, 0x0710); 
writeRegister(ILI9225_DISP_CTRL1, 0x0012); 
delay(50); 
writeRegister(ILI9225_DISP_CTRL1, 0x1017);
SPI.endTransaction();
setRotation();
fillScreen();
}

/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!
 
Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include "glcdfont.c"

// Draw a circle outline
void ILI9225_t3::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
int16_t f = r;
int16_t ddF_x = 1;
int16_t ddF_y = -2 * r;
int16_t x = 0;
int16_t y = r;
drawPixel(x0 - r, y0, color); // top left
drawPixel(x0, y0 + r, color); // top right
drawPixel(x0, y0-  r, color); // bottom right
drawPixel(x0 + r, y0, color); // bottom left

  while (x<y)
  {
    if (f >= 0)
	{
    y--;
    ddF_y += 2;
    f += ddF_y;
    }
  x++;
  ddF_x += 2;
  f += ddF_x;

  drawPixel(x0 + x, y0 + y, color);
  drawPixel(x0 - x, y0 + y, color);
  drawPixel(x0 + x, y0 - y, color);
  drawPixel(x0 - x, y0 - y, color);
  drawPixel(x0 + y, y0 + x, color);
  drawPixel(x0 - y, y0 + x, color);
  drawPixel(x0 + y, y0 - x, color);
  drawPixel(x0 - y, y0 - x, color);
  }
}

void ILI9225_t3::drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
int16_t f     = r;
int16_t ddF_x = 1;
int16_t ddF_y = -2 * r;
int16_t x     = 0;
int16_t y     = r;

  while (x<y)
  {
    if (f >= 0)
	{
    y--;
    ddF_y += 2;
    f += ddF_y;
    }
  x++;
  ddF_x += 2;
  f += ddF_x;
    if (cornername & 0x4)
    {
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 + y, y0 + x, color);
    } 
    if (cornername & 0x2)
    {
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8)
    {
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1)
    {
    drawPixel(x0 - y, y0 - x, color);
    drawPixel(x0 - x, y0 - y, color);
    }
  }
}

void ILI9225_t3::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
drawFastVLine(x0, y0-r, 2*r, color);
fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void ILI9225_t3::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
int16_t f     = r;
int16_t ddF_x = 1;
int16_t ddF_y = -2 * r;
int16_t x     = 0;
int16_t y     = r;

  while (x<y)
  {
    if (f >= 0)
	{
    y--;
    ddF_y += 2;
    f += ddF_y;
    }
  x++;
  ddF_x += 2;
  f += ddF_x;
    if (cornername & 0x1)
	{
    drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
    drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2)
	{
    drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
    drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

// Bresenham's algorithm - thx wikpedia
void ILI9225_t3::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
  if (y0 == y1)
  {
    if (x1 > x0)
	drawFastHLine(x0, y0, x1 - x0, color);
	else if (x1 < x0)
	drawFastHLine(x1, y0, x0 - x1, color);
	else
	drawPixel(x0, y0, color);
  return;
  }
  else if (x0 == x1)
  {
	if (y1 > y0)
	drawFastVLine(x0, y0, y1 - y0, color);
	else
	drawFastVLine(x0, y1, y0 - y1, color);
  return;
  }
bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep)
  {
  swap(x0, y0);
  swap(x1, y1);
  }
  if (x0 > x1)
  {
  swap(x0, x1);
  swap(y0, y1);
  }

int16_t dx, dy;
dx = x1 - x0;
dy = abs(y1 - y0);
int16_t err = dx / 2;
int16_t ystep;

  if (y0 < y1)
  ystep = 1;
  else
  ystep = -1;

int16_t xbegin = x0;
  if (steep)
  {
	for (; x0<=x1; x0++)
	{
	err -= dy;
	  if (err < 0)
	  {
	  int16_t len = x0 - xbegin;
	    if (len)
	    drawFastVLine(y0, xbegin, len + 1, color);
	    else
	    drawPixel(y0, x0, color);
	  xbegin = x0 + 1;
	  y0 += ystep;
	  err += dx;
	  }
	}
	if (x0 > xbegin + 1)
	drawFastVLine(y0, xbegin, x0 - xbegin, color);
  }
  else
  {
	for (; x0<=x1; x0++)
	{
	err -= dy;
	  if (err < 0)
	  {
	  int16_t len = x0 - xbegin;
	    if (len)
	    drawFastHLine(xbegin, y0, len + 1, color);
	    else
	    drawPixel(x0, y0, color);
	  xbegin = x0 + 1;
	  y0 += ystep;
	  err += dx;
	  }
	}
	if (x0 > xbegin + 1)
	drawFastHLine(xbegin, y0, x0 - xbegin, color);
  }
}

// Draw a rectangle
void ILI9225_t3::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
drawFastHLine(x, y, w, color);
drawFastHLine(x, y+h, w, color);
drawFastVLine(x, y, h, color);
drawFastVLine(x+w, y, h, color);
}

// Draw a rounded rectangle
void ILI9225_t3::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
// smarter version
drawFastHLine(x+r  , y    , w-2*r, color); // Top
drawFastHLine(x+r  , y+h, w-2*r, color); // Bottom
drawFastVLine(x    , y+r  , h-2*r, color); // Left
drawFastVLine(x+w, y+r  , h-2*r, color); // Right
// draw four corners
drawCircleHelper(x+r    , y+r    , r, 1, color);
drawCircleHelper(x+w-r, y+r    , r, 2, color);
drawCircleHelper(x+w-r, y+h-r, r, 4, color);
drawCircleHelper(x+r    , y+h-r, r, 8, color);
}

// Fill a rounded rectangle
void ILI9225_t3::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
// smarter version
fillRect(x+r, y+1, w-2*r, h, color);
// draw four corners
fillCircleHelper(x+w-r, y+r, r, 1, h-2*r, color);
fillCircleHelper(x+r    , y+r, r, 2, h-2*r, color);
}

// Draw a triangle
void ILI9225_t3::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
drawLine(x0, y0, x1, y1, color);
drawLine(x1, y1, x2, y2, color);
drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void ILI9225_t3::fillTriangle ( int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
int16_t a, b, y, last;
// Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1)
  {swap(y0, y1); swap(x0, x1);}
  if (y1 > y2)
  {swap(y2, y1); swap(x2, x1);}
  if (y0 > y1)
  {swap(y0, y1); swap(x0, x1);}

  if(y0 == y2) // Handle awkward all-on-same-line case as its own thing
  {
  a = b = x0;
    if(x1 < a)
	a = x1;
    else if(x1 > b)
	b = x1;
    if(x2 < a)
	a = x2;
    else if(x2 > b)
	b = x2;
  drawFastHLine(a, y0, b-a+1, color);
  return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2)
  last = y1;   // Include y1 scanline
  else
  last = y1-1; // Skip it

  for(y=y0; y<=last; y++)
  {
  a   = x0 + sa / dy01;
  b   = x0 + sb / dy02;
  sa += dx01;
  sb += dx02;

  /* longhand:
  a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
  b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
  */

  if(a > b)
    swap(a,b);
  drawFastHLine(a, y, b-a, color);
  }
// For lower part of triangle, find scanline crossings for segments
// 0-2 and 1-2.  This loop is skipped if y1=y2.
sa = dx12 * (y - y1);
sb = dx02 * (y - y0);
  for(; y<=y2; y++)
  {
  a   = x1 + sa / dy12;
  b   = x0 + sb / dy02;
  sa += dx12;
  sb += dx02;

  /* longhand:
  a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
  b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
  */

  if(a > b)
    swap(a,b);
  drawFastHLine(a, y, b-a, color);
  }
}


void ILI9225_t3::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{
int16_t i, j, byteWidth = (w + 7) / 8;
  for(j=0; j<h; j++)
  {
    for(i=0; i<w; i++ )
	{
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)))
	  drawPixel(x+i, y+j, color);
    }
  }
}

size_t ILI9225_t3::write(uint8_t c)
{
  if (c == '\n')
  {
  cursor_y += textsize*8;
  cursor_x  = 0;
  }
  else if (c == '\r')
  {// skip em
  }
  else
  {
  drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
  cursor_x += textsize*6;
    if (wrap && (cursor_x > (_width - textsize*6)))
	{
    cursor_y += textsize*8+2;
    cursor_x = 0;
    }
  }
  return 1;
}

// Draw a character
void ILI9225_t3::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t fgcolor, uint16_t bgcolor, uint8_t size)
{
  if((x >= _width)            || // Clip right
  (y >= _height)           || // Clip bottom
  ((x + 6 * size - 1) < 0) || // Clip left  TODO: is this correct?
  ((y + 8 * size - 1) < 0))   // Clip top   TODO: is this correct?
  return;

  if (fgcolor == bgcolor)
  {
  // This transparent approach is only about 20% faster
    if (size == 1)
    {
	uint8_t mask = 0x01;
	int16_t xoff, yoff;
	  for (yoff=0; yoff < 8; yoff++)
      {
      uint8_t line = 0;
        for (xoff=0; xoff < 5; xoff++)
	    {
	      if (font[c * 5 + xoff] & mask) line |= 1;
          line <<= 1;
	    }
      line >>= 1;
      xoff = 0;
        while (line)
	    {
          if (line == 0x1F)
		  {
          drawFastHLine(x + xoff, y + yoff, 5, fgcolor);
          break;
          }
		  else if (line == 0x1E)
		  {
		  drawFastHLine(x + xoff, y + yoff, 4, fgcolor);
		  break;
		  }
		  else if ((line & 0x1C) == 0x1C)
		  {
		  drawFastHLine(x + xoff, y + yoff, 3, fgcolor);
		  line <<= 4;
		  xoff += 4;
		  }
		  else if ((line & 0x18) == 0x18)
		  {
		  drawFastHLine(x + xoff, y + yoff, 2, fgcolor);
		  line <<= 3;
		  xoff += 3;
		  }
		  else if ((line & 0x10) == 0x10)
		  {
		  drawPixel(x + xoff, y + yoff, fgcolor);
		  line <<= 2;
		  xoff += 2;
		  }
		  else
		  {
		  line <<= 1;
		  xoff += 1;
		  }
	    }
      mask = mask << 1;
      }
    }
    else
    {
    uint8_t mask = 0x01;
    int16_t xoff, yoff;
	  for (yoff=0; yoff < 8; yoff++)
	  {
	  uint8_t line = 0;
	    for (xoff=0; xoff < 5; xoff++)
	    {
		  if (font[c * 5 + xoff] & mask) line |= 1;
		  line <<= 1;
	    }
	  line >>= 1;
	  xoff = 0;
	    while (line)
	    {
	      if (line == 0x1F)
		  {
		  fillRect(x + xoff * size, y + yoff * size,
		  5 * size, size, fgcolor);
		  break;
		  }
		  else if (line == 0x1E)
		  {
		  fillRect(x + xoff * size, y + yoff * size,
		  4 * size, size, fgcolor);
		  break;
		  }
		  else if ((line & 0x1C) == 0x1C)
		  {
		  fillRect(x + xoff * size, y + yoff * size,
		  3 * size, size, fgcolor);
		  line <<= 4;
		  xoff += 4;
		  }
		  else if ((line & 0x18) == 0x18)
		  {
		  fillRect(x + xoff * size, y + yoff * size,
		  2 * size, size, fgcolor);
		  line <<= 3;
		  xoff += 3;
		  }
		  else if ((line & 0x10) == 0x10)
		  {
		  fillRect(x + xoff * size, y + yoff * size,
		  size, size, fgcolor);
		  line <<= 2;
		  xoff += 2;
		  }
		  else
		  {
		  line <<= 1;
		  xoff += 1;
		  }
	    }
	  mask = mask << 1;
      }
    }
  }
  else
  {
  // This solid background approach is about 5 time faster
  uint8_t xr, yr;
  uint8_t mask = 0x01;
  uint16_t color;
  setAddrWindow(x, y, x + 6 * size-1, y + 8 * size);
	for (y=0; y < 8; y++)
	{
	  for (yr=0; yr < size; yr++)
	  {
		for (x=0; x < 5; x++)
		{
		  if (font[c * 5 + x] & mask)
		  color = fgcolor;
		  else
		  color = bgcolor;
	      for (xr=0; xr < size; xr++)
		  writedata16_cont(color);
		}
	    for (xr=0; xr < size; xr++)
		writedata16_cont(bgcolor);
	  }
    mask = mask << 1;
    }
  SPI.endTransaction();
  }
}

void ILI9225_t3::setCursor(int16_t x, int16_t y)
{
cursor_x = x;
cursor_y = y;
}

void ILI9225_t3::setTextSize(uint8_t s)
{
textsize = (s > 0) ? s : 1;
}

void ILI9225_t3::setTextColor(uint16_t c)
{
// For 'transparent' background, we'll set the bg 
// to the same as fg instead of using a flag
textcolor = textbgcolor = c;
}

void ILI9225_t3::setTextColor(uint16_t c, uint16_t b)
{
textcolor   = c;
textbgcolor = b; 
}

void ILI9225_t3::setTextWrap(boolean w)
{
wrap = w;
}

uint8_t ILI9225_t3::getRotation(void)
{
return rotation;
}
