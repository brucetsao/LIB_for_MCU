// https://github.com/PaulStoffregen/ILI9341_t3
// http://forum.pjrc.com/threads/26305-Highly-optimized-ILI9341-(320x240-TFT-color-display)-library

/***************************************************
  This is our library for the Adafruit  ILI9341 Breakout and Shield
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

#ifndef _ILI9225_t3H_
#define _ILI9225_t3H_

#include "Arduino.h"

#define ILI9225_TFTWIDTH  176
#define ILI9225_TFTHEIGHT 220

#define ILI9225_DRIVER_OUTPUT_CTRL      (0x01u)  // Driver Output Control
#define ILI9225_LCD_AC_DRIVING_CTRL     (0x02u)  // LCD AC Driving Control
#define ILI9225_ENTRY_MODE            	(0x03u)  // Entry Mode
#define ILI9225_DISP_CTRL1          	(0x07u)  // Display Control 1
#define ILI9225_DISP_CTRL2				(0x08u)  // Blank Period Control
#define ILI9225_FRAME_CYCLE_CTRL        (0x0Bu)  // Frame Cycle Control
#define ILI9225_INTERFACE_CTRL          (0x0Cu)  // Interface Control
#define ILI9225_OSC_CTRL             	(0x0Fu)  // Osc Control
#define ILI9225_POWER_CTRL1            	(0x10u)  // Power Control 1
#define ILI9225_POWER_CTRL2           	(0x11u)  // Power Control 2
#define ILI9225_POWER_CTRL3            	(0x12u)  // Power Control 3
#define ILI9225_POWER_CTRL4            	(0x13u)  // Power Control 4
#define ILI9225_POWER_CTRL5            	(0x14u)  // Power Control 5
#define ILI9225_VCI_RECYCLING          	(0x15u)  // VCI Recycling
#define ILI9225_RAM_ADDR_SET1           (0x20u)  // Horizontal GRAM Address Set
#define ILI9225_RAM_ADDR_SET2           (0x21u)  // Vertical GRAM Address Set
#define ILI9225_GRAM_DATA_REG           (0x22u)  // GRAM Data Register
#define ILI9225_GATE_SCAN_CTRL          (0x30u)  // Gate Scan Control Register
#define ILI9225_VERTICAL_SCROLL_CTRL1   (0x31u)  // Vertical Scroll Control 1 Register
#define ILI9225_VERTICAL_SCROLL_CTRL2   (0x32u)  // Vertical Scroll Control 2 Register
#define ILI9225_VERTICAL_SCROLL_CTRL3   (0x33u)  // Vertical Scroll Control 3 Register
#define ILI9225_PARTIAL_DRIVING_POS1    (0x34u)  // Partial Driving Position 1 Register
#define ILI9225_PARTIAL_DRIVING_POS2    (0x35u)  // Partial Driving Position 2 Register
#define ILI9225_HORIZONTAL_WINDOW_ADDR1 (0x36u)  // Horizontal Address Start Position
#define ILI9225_HORIZONTAL_WINDOW_ADDR2	(0x37u)  // Horizontal Address End Position
#define ILI9225_VERTICAL_WINDOW_ADDR1   (0x38u)  // Vertical Address Start Position
#define ILI9225_VERTICAL_WINDOW_ADDR2   (0x39u)  // Vertical Address End Position
#define ILI9225_GAMMA_CTRL1            	(0x50u)  // Gamma Control 1
#define ILI9225_GAMMA_CTRL2             (0x51u)  // Gamma Control 2
#define ILI9225_GAMMA_CTRL3            	(0x52u)  // Gamma Control 3
#define ILI9225_GAMMA_CTRL4            	(0x53u)  // Gamma Control 4
#define ILI9225_GAMMA_CTRL5            	(0x54u)  // Gamma Control 5
#define ILI9225_GAMMA_CTRL6            	(0x55u)  // Gamma Control 6
#define ILI9225_GAMMA_CTRL7            	(0x56u)  // Gamma Control 7
#define ILI9225_GAMMA_CTRL8            	(0x57u)  // Gamma Control 8
#define ILI9225_GAMMA_CTRL9             (0x58u)  // Gamma Control 9
#define ILI9225_GAMMA_CTRL10            (0x59u)  // Gamma Control 10

/* RGB 24-bits color table definition (RGB888). */
#define RGB888_RGB565(color) ((((color) >> 19) & 0x1f) << 11) | ((((color) >> 10) & 0x3f) << 5) | (((color) >> 3) & 0x1f)

#define COLOR_BLACK          RGB888_RGB565(0x000000u)
#define COLOR_WHITE          RGB888_RGB565(0xFFFFFFu)
#define COLOR_BLUE           RGB888_RGB565(0x0000FFu)
#define COLOR_GREEN          RGB888_RGB565(0x00FF00u)
#define COLOR_RED            RGB888_RGB565(0xFF0000u)
#define COLOR_NAVY           RGB888_RGB565(0x000080u)
#define COLOR_DARKBLUE       RGB888_RGB565(0x00008Bu)
#define COLOR_DARKGREEN      RGB888_RGB565(0x006400u)
#define COLOR_DARKCYAN       RGB888_RGB565(0x008B8Bu)
#define COLOR_CYAN           RGB888_RGB565(0x00FFFFu)
#define COLOR_TURQUOISE      RGB888_RGB565(0x40E0D0u)
#define COLOR_INDIGO         RGB888_RGB565(0x4B0082u)
#define COLOR_DARKRED        RGB888_RGB565(0x800000u)
#define COLOR_OLIVE          RGB888_RGB565(0x808000u)
#define COLOR_GRAY           RGB888_RGB565(0x808080u)
#define COLOR_SKYBLUE        RGB888_RGB565(0x87CEEBu)
#define COLOR_BLUEVIOLET     RGB888_RGB565(0x8A2BE2u)
#define COLOR_LIGHTGREEN     RGB888_RGB565(0x90EE90u)
#define COLOR_DARKVIOLET     RGB888_RGB565(0x9400D3u)
#define COLOR_YELLOWGREEN    RGB888_RGB565(0x9ACD32u)
#define COLOR_BROWN          RGB888_RGB565(0xA52A2Au)
#define COLOR_DARKGRAY       RGB888_RGB565(0xA9A9A9u)
#define COLOR_SIENNA         RGB888_RGB565(0xA0522Du)
#define COLOR_LIGHTBLUE      RGB888_RGB565(0xADD8E6u)
#define COLOR_GREENYELLOW    RGB888_RGB565(0xADFF2Fu)
#define COLOR_SILVER         RGB888_RGB565(0xC0C0C0u)
#define COLOR_LIGHTGREY      RGB888_RGB565(0xD3D3D3u)
#define COLOR_LIGHTCYAN      RGB888_RGB565(0xE0FFFFu)
#define COLOR_VIOLET         RGB888_RGB565(0xEE82EEu)
#define COLOR_AZUR           RGB888_RGB565(0xF0FFFFu)
#define COLOR_BEIGE          RGB888_RGB565(0xF5F5DCu)
#define COLOR_MAGENTA        RGB888_RGB565(0xFF00FFu)
#define COLOR_TOMATO         RGB888_RGB565(0xFF6347u)
#define COLOR_GOLD           RGB888_RGB565(0xFFD700u)
#define COLOR_ORANGE         RGB888_RGB565(0xFFA500u)
#define COLOR_SNOW           RGB888_RGB565(0xFFFAFAu)
#define COLOR_YELLOW         RGB888_RGB565(0xFFFF00u)

class ILI9225_t3 : public Print
{
  public:
    ILI9225_t3(uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12, uint8_t _LED=255);
	void begin(void);
	void pushColor(uint16_t color);
	void drawPixel(uint16_t x1, uint16_t y1, uint16_t color);
	void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	void fillScreen(uint16_t color=COLOR_BLACK);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	void setRotation(uint8_t r=0, uint8_t i=0);
	void invertDisplay(boolean i);
	void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	uint8_t getRotation(void);
	void setBacklight(uint8_t val);
	void setDisplay(boolean flag);
	// Pass 8-bit (each) R,G,B, get back 16-bit packed color
	static uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3); }

	// from Adafruit_GFX.h
	void drawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);  
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
	void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
	void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
	void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
	void setCursor(int16_t x, int16_t y);
	void setTextColor(uint16_t c);
	void setTextColor(uint16_t c, uint16_t bg);
	void setTextSize(uint8_t s);
	void setTextWrap(boolean w);
	virtual size_t write(uint8_t c);
	int16_t width(void) { return _width; }
	int16_t height(void) { return _height; }
	void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
	void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
//    uint8_t setCS(uint8_t pin);


  protected:
    int16_t _width, _height, cursor_x, cursor_y; // Display w/h as modified by current rotation
    uint16_t textcolor, textbgcolor;
    uint8_t textsize, rotation;
    boolean wrap; // If set, 'wrap' text at right edge of display
	uint8_t startH, endH, startV, endV, ramAddrOne, ramAddrTwo;
	
  	uint8_t  _cs, _dc, _rst, _mosi, _sclk, _miso, _led;
    int16_t pcs_data, pcs_command;

	void waitFifoNotFull(void)
	{
	uint32_t sr;
	uint32_t tmp __attribute__((unused));
	  do
	  {
	  sr = KINETISK_SPI0.SR;
	    if(sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
	  }
	  while((sr & (15 << 12)) > (3 << 12));
	}
	void waitFifoEmpty(void)
	{
	uint32_t sr;
	uint32_t tmp __attribute__((unused));
	  do
	  {
	  sr = KINETISK_SPI0.SR;
	    if(sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
	  }
	  while((sr & 0xF0F0) > 0);             // wait both RX & TX empty
	}
	void waitTransmitComplete(void) __attribute__((always_inline))
	{
	uint32_t tmp __attribute__((unused));
	  while(!(KINETISK_SPI0.SR & SPI_SR_TCF)); // wait until final output done
	tmp = KINETISK_SPI0.POPR;                  // drain the final RX FIFO word
	}
	void waitTransmitComplete(uint32_t mcr) __attribute__((always_inline))
	{
	uint32_t tmp __attribute__((unused));
	  while(1)
	  {
	  uint32_t sr = KINETISK_SPI0.SR;
		if (sr & SPI_SR_EOQF) break;  // wait for last transmit
		if (sr &  0xF0) tmp = KINETISK_SPI0.POPR;
	  }
	KINETISK_SPI0.SR = SPI_SR_EOQF;
	SPI0_MCR = mcr;
	  while(KINETISK_SPI0.SR & 0xF0)
	  {
	  tmp = KINETISK_SPI0.POPR;
	  }
	}
	void writecommand_cont(uint8_t c) __attribute__((always_inline))
	{
	KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
	waitFifoNotFull();
	}
	void writedata8_cont(uint8_t c) __attribute__((always_inline))
	{
	KINETISK_SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
	waitFifoNotFull();
	}
	void writedata16_cont(uint16_t d) __attribute__((always_inline))
	{
	KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
	waitFifoNotFull();
	}
	void writecommand_last(uint8_t c) __attribute__((always_inline))
	{
	uint32_t mcr = SPI0_MCR;
	KINETISK_SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
	waitTransmitComplete(mcr);
	}
	void writedata8_last(uint8_t c) __attribute__((always_inline))
	{
	uint32_t mcr = SPI0_MCR;
	KINETISK_SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
	waitTransmitComplete(mcr);
	}
	void writedata16_last(uint16_t d) __attribute__((always_inline))
	{
	uint32_t mcr = SPI0_MCR;
	KINETISK_SPI0.PUSHR = d | (pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_EOQ;
	waitTransmitComplete(mcr);
	}
	void writeRegister(uint16_t reg, uint16_t data) __attribute__((always_inline))
	{
	writecommand_cont(reg);
	writedata16_cont(data);
	}
	
	uint8_t setCS(uint8_t pin)
	{
	switch (pin) {
	  case 10: CORE_PIN10_CONFIG = PORT_PCR_MUX(2) | PORT_PCR_SRE; return 0x01; // PTC4
	  case 2:  CORE_PIN2_CONFIG  = PORT_PCR_MUX(2) | PORT_PCR_SRE; return 0x01; // PTD0
	  case 9:  CORE_PIN9_CONFIG  = PORT_PCR_MUX(2) | PORT_PCR_SRE; return 0x02; // PTC3
	  case 6:  CORE_PIN6_CONFIG  = PORT_PCR_MUX(2) | PORT_PCR_SRE; return 0x02; // PTD4
	  case 20: CORE_PIN20_CONFIG = PORT_PCR_MUX(2) | PORT_PCR_SRE; return 0x04; // PTD5
	  case 23: CORE_PIN23_CONFIG = PORT_PCR_MUX(2) | PORT_PCR_SRE; return 0x04; // PTC2
	  case 21: CORE_PIN21_CONFIG = PORT_PCR_MUX(2) | PORT_PCR_SRE; return 0x08; // PTD6
	  case 22: CORE_PIN22_CONFIG = PORT_PCR_MUX(2) | PORT_PCR_SRE; return 0x08; // PTC1
	  case 15: CORE_PIN15_CONFIG = PORT_PCR_MUX(2) | PORT_PCR_SRE; return 0x10; // PTC0
	}
	return 0;
}

};
#ifndef swap
#define swap(a, b) { typeof(a) t = a; a = b; b = t; }
#endif

#endif
