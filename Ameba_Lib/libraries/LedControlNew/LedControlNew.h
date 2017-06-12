/*
  LedControl.h - A library for controling Led Digits or 
  Led arrays with a MAX7219/MAX7221
  Copyright (c) 2007 Eberhard Fahle

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef LedControlNew_h
#define LedControlNew_h

//#include <WConstants.h>
#include "Arduino.h"



#define MAXDEVIESNUM 12
#define maxDisplayChar 16 
// standard ascii 5x7 font
// defines ascii characters 0x20-0x7F (32-127)
static  unsigned char Font5x7[][5] = {
   {0x00,0x00,0x00,0x00,0x00},  /*space*/ // is 32 in ASCII
   {0x00,0xF6,0xF6,0x00,0x00},  /*!*/
   {0x00,0xE0,0x00,0xE0,0x00},  /*"*/
   {0x28,0xFE,0x28,0xFE,0x28},  /*#*/
   {0x00,0x64,0xD6,0x54,0x08},  /*$*/
   {0xC2,0xCC,0x10,0x26,0xC6},  /*%*/
   {0x4C,0xB2,0x92,0x6C,0x0A},  /*&*/
   {0x00,0x00,0xE0,0x00,0x00},  /*'*/
   {0x00,0x38,0x44,0x82,0x00},  /*(*/
   {0x00,0x82,0x44,0x38,0x00},  /*)*/
   {0x88,0x50,0xF8,0x50,0x88},  /***/
   {0x08,0x08,0x3E,0x08,0x08},  /*+*/
   {0x00,0x00,0x05,0x06,0x00},  /*,*/
   {0x08,0x08,0x08,0x08,0x08},  /*-*/
   {0x00,0x00,0x06,0x06,0x00},  /*.*/
   {0x02,0x0C,0x10,0x60,0x80},  /*/*/
   {0x7C,0x8A,0x92,0xA2,0x7C},  /*0*/
   {0x00,0x42,0xFE,0x02,0x00},  /*1*/
   {0x42,0x86,0x8A,0x92,0x62},  /*2*/
   {0x44,0x82,0x92,0x92,0x6C},  /*3*/
   {0x10,0x30,0x50,0xFE,0x10},  /*4*/
   {0xE4,0xA2,0xA2,0xA2,0x9C},  /*5*/
   {0x3C,0x52,0x92,0x92,0x0C},  /*6*/
   {0x80,0x86,0x98,0xE0,0x80},  /*7*/
   {0x6C,0x92,0x92,0x92,0x6C},  /*8*/
   {0x60,0x92,0x92,0x94,0x78},  /*9*/
   {0x00,0x00,0x36,0x36,0x00},  /*:*/
   {0x00,0x00,0x35,0x36,0x00},  /*;*/
   {0x10,0x28,0x44,0x82,0x00},  /*<*/
   {0x28,0x28,0x28,0x28,0x28},  /*=*/
   {0x00,0x82,0x44,0x28,0x10},  /*>*/
   {0x40,0x80,0x8A,0x90,0x60},  /*?*/
   {0x7C,0x82,0xBA,0xBA,0x62},  /*@*/
   {0x3E,0x48,0x88,0x48,0x3E},  /*A*/
   {0xFE,0x92,0x92,0x92,0x6C},  /*B*/
   {0x7C,0x82,0x82,0x82,0x44},  /*C*/
   {0xFE,0x82,0x82,0x82,0x7C},  /*D*/   
   {0xFE,0x92,0x92,0x92,0x82},  /*E*/
   {0xFE,0x90,0x90,0x90,0x80},  /*F*/
   {0x7C,0x82,0x82,0x8A,0x4E},  /*G*/
   {0xFE,0x10,0x10,0x10,0xFE},  /*H*/  
   {0x82,0x82,0xFE,0x82,0x82},  /*I*/
   {0x84,0x82,0xFC,0x80,0x80},  /*J*/
   {0xFE,0x10,0x28,0x44,0x82},  /*K*/
   {0xFE,0x02,0x02,0x02,0x02},  /*L*/ 
   {0xFE,0x40,0x20,0x40,0xFE},  /*M*/
   {0xFE,0x60,0x10,0x0C,0xFE},  /*N*/
   {0x7C,0x82,0x82,0x82,0x7C},  /*O*/
   {0xFE,0x90,0x90,0x90,0x60},  /*P*/
   {0x7C,0x82,0x82,0x86,0x7E},  /*Q*/
   {0xFE,0x90,0x98,0x94,0x62},  /*R*/
   {0x64,0x92,0x92,0x92,0x4C},  /*S*/
   {0x80,0x80,0xFE,0x80,0x80},  /*T*/
   {0xFC,0x02,0x02,0x02,0xFC},  /*U*/
   {0xF8,0x04,0x02,0x04,0xF8},  /*V*/
   {0xFC,0x02,0x0C,0x02,0xFC},  /*W*/
   {0xC6,0x28,0x10,0x28,0xC6},  /*X*/
   {0xC0,0x20,0x1E,0x20,0xC0},  /*Y*/
   {0x86,0x8A,0x92,0xA2,0xC2},  /*Z*/    
   {0x00,0x00,0xFE,0x82,0x00},  /*[*/
   {0x00,0x00,0x00,0x00,0x00},  /*this should be / */
   {0x80,0x60,0x10,0x0C,0x02},  /*]*/
   {0x20,0x40,0x80,0x40,0x20},  /*^*/
   {0x02,0x02,0x02,0x02,0x02},  /*_*/ // use for 7 dot high display
   {0x80,0x40,0x20,0x00,0x00},  /*`*/
   {0x04,0x2A,0x2A,0x2A,0x1E},  /*a*/
   {0xFE,0x12,0x22,0x22,0x1C},  /*b*/
   {0x1C,0x22,0x22,0x22,0x14},  /*c*/
   {0x1C,0x22,0x22,0x12,0xFE},  /*d*/
   {0x1C,0x2A,0x2A,0x2A,0x18},  /*e*/
   {0x10,0x7E,0x90,0x80,0x40},  /*f*/
   {0x18,0x25,0x25,0x25,0x1E},  /*g*/
   {0xFE,0x10,0x10,0x10,0x0E},  /*h*/
   {0x00,0x12,0x5E,0x02,0x00},  /*i*/
   {0x02,0x01,0x01,0x11,0x5E},  /*j*/
   {0xFE,0x08,0x08,0x14,0x22},  /*k*/
   {0x00,0x82,0xFE,0x02,0x00},  /*l*/
   {0x3E,0x20,0x1C,0x20,0x1E},  /*m*/
   {0x3E,0x20,0x20,0x20,0x1E},  /*n*/
   {0x1C,0x22,0x22,0x22,0x1C},  /*o*/
   {0x3F,0x24,0x24,0x24,0x18},  /*p*/
   {0x18,0x24,0x24,0x3F,0x01},  /*q*/
   {0x3E,0x10,0x20,0x20,0x10},  /*r*/
   {0x12,0x2A,0x2A,0x2A,0x04},  /*s*/
   {0x00,0x10,0x3C,0x12,0x04},  /*t*/
   {0x3C,0x02,0x02,0x02,0x3E},  /*u*/
   {0x30,0x0C,0x02,0x0C,0x30},  /*v*/
   {0x38,0x06,0x18,0x06,0x38},  /*w*/
   {0x22,0x14,0x08,0x14,0x22},  /*x*/
   {0x38,0x05,0x05,0x05,0x3E},  /*y*/
   {0x22,0x26,0x2A,0x32,0x22},  /*z*/
   {0x00,0x10,0x6C,0x82,0x82},  /*{*/
   {0x00,0x00,0xFF,0x00,0x00},  /*|*/
   //{0x04,0x02,0xFF,0x02,0x04},  /*|, down arrow*/
   {0x82,0x82,0x6C,0x10,0x00},  /*}*/
   {0x08,0x10,0x18,0x08,0x10},  /*~*/
 };



/*
 * Here are the segments to be switched on for characters and digits on
 * 7-Segment Displays
 */
const static byte charTable[128] = {
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
    B01111111,B01111011,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B10000000,B00000001,B10000000,B00000000,
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
    B01111111,B01111011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,B00000000,
    B00110111,B00000000,B00000000,B00000000,B00001110,B00000000,B00000000,B00000000,
    B01100111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001000,
    B00000000,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,B00000000,
    B00110111,B00000000,B00000000,B00000000,B00001110,B00000000,B00000000,B00000000,
    B01100111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};

class LedControlNew {
    private :
    /* We keep track of the led-status for 8 devices in this array */
 //   byte status[64];
    /* This is the array we send the data with 2 commandbytes for 8 devices */
    byte spidata[MAXDEVIESNUM*2];
    /* Data is shifted out of this pin*/
    int SPI_MOSI;
    /* The clock is signaled on this pin */
    int SPI_CLK;
    /* This one is driven LOW for chip selectzion */
    int SPI_CS;
    /* The maximum number of devices to be used */
    int maxDevices;
	int DisplayWidth ;
    /* Send out a single command to the device */
    void spiTransfer(int addr, byte opcode, byte data);

	byte DisplayContent[maxDisplayChar][8] ;
    
    public:
    byte status[MAXDEVIESNUM*8];
    /* 
     * Create a new controler 
     * Params :
     * int dataPin	The pin on the Arduino where data gets shifted out
     * int clockPin	The pin for the clock
     * int csPin	The pin for selecting the device when data is to be sent
     * int numDevices	The maximum number of devices that can be controled+
     */
    LedControlNew(int dataPin, int clkPin, int csPin, int numDevices);

    /*
     * Gets the maximum number of devices attached to
     * this LedControl.
     * Returns :
     * int the number of devices attached to this LedControl
     */
    int getDeviceCount();

    /* 
     * Set the shutdown (power saving) mode for the device
     * Params :
     * int addr		The address of the display to control
     * boolean b	If true the device goes into power-down mode. If false
     *			device goes into normal operation
     */
    void shutdown(int addr, bool b);

    /* 
     * Set the number of digits (or rows) to be displayed.
     * See datasheet for sideeffects of the scanlimit on the brightness
     * of the display.
     * Params :
     * int addr		The address of the display to control
     * int limit The number of digits to be displayed
     * Only values between 0 (only 1 digit) and 7 (all digits) are valid.
     */
    void setScanLimit(int addr, int limit);

    /* 
     * Set the brightness of the display.
     * Params:
     * int addr		The address of the display to control
     * int intensity the brightness of the display. 
     * Only values between 0(darkest) and 15(brightest) are valid.
     */
    void setIntensity(int addr, int intensity);

    /* 
     * Switch all Leds on the display off. 
     * Params:
     * int addr		The address of the display to control
     */
    void clearDisplay(int addr);

    /* 
     * Set the status for a specific Led.
     * Params :
     * int addr		The address of the display to control
     * int row		the row in which the led is located 
     *			Only values between 0 and 7 are valid.
     * int col		the column in which the led is located
     *			Only values between 0 and 7 are valid.
     * boolean state	If true the led is switched on, if false it is switched off
     */
    void setLed(int addr, int row, int col, boolean state);

    /* 
     * Set the 8 Led's in a row to a new state
     * Params:
     * int addr		The address of the display to control
     * int row		The row on which the led's are to be set
     *			Only values between 0 and 7 are valid.
     * byte value	A bit set to 1 in this value will light up the
     *			corresponding led.
     */
    void setRow(int addr, int row, byte value);

    /* 
     * Set the 8 Led's in a column to a new state
     * Params:
     * int addr		The address of the display to control
     * int row		The row on which the led's are to be set
     *			Only values between 0 and 7 are valid.
     * byte value	A bit set to 1 in this value will light up the
     *			corresponding led.
     */
    void setColumn(int addr, int col, byte value);

    /* 
     * Display a hexadecimal digit on a 7-Segment Display
     * Params:
     * int addr		the address of the display to control
     * int digit	the position of the digit on the display
     * byte value	the value to be displayed. 
     *			Only values between 0x00 and 0x0F are valid.
     * boolean dp	If true also switches on the decimal point.
     */
    void setDigit(int addr, int digit, byte value, boolean dp);

    /* 
     * Display a character on a 7-Segment display.
     * The char that can be acutally displayed are obviously quite limited.
     * Here is the whole set :	
     *	'0','1','2','3','4','5','6','7','8','9','0',
     *  'A','b','c','d','E','F','H','L','P',
     *  '.','-','_',' ' 
     * Params:
     * int addr		the address of the display to control
     * int digit	the position of the character on the display
     * char value	the character to be displayed. (See the limited set above!) 
     * boolean dp	If true also switches on the decimal point.
     */
    void setChar(int addr, int digit, char value, boolean dp);


 //   void setCharFont5X7(int addr, int digit, char value);
 //   void setStringFont5X7(int addr, int digit, String str, int fontwidth);
//    void LeftRotate(int offset); 
	void ClearDisplayContent()  ;
	void setDisplayContent(String str)  ;
	void CharDisplay()  ;


};

#endif	//LedControlNew.h



