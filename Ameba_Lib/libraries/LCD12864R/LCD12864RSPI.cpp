/*
	Class to operate the LCD12864 screen using SPI interface.
	Equiptment wiki: http://www.dfrobot.com/wiki/index.php?title=SPI_LCD_Module_(SKU:DFR0091)
	The classes are modified version from the library downloaded in the above wiki page.
	
	Author: Yi Wei.
	Date: 2012-04-29
*/

#include "arduino.h"
#include "LCD12864RSPI.h"

extern "C" 
{
	#include <inttypes.h>
	#include <stdio.h>  //not needed yet
	#include <string.h> //needed for strlen()
	#include <avr/pgmspace.h>
}


LCD12864RSPI::LCD12864RSPI() 
{
	this->DEFAULTTIME = 80; // 80 ms default time
	this->delayTime = DEFAULTTIME;
} 

/* Delay dalayTime amount of microseconds. */
void LCD12864RSPI::delayns(void)
{   
	delayMicroseconds(delayTime);
}

/* Write byte data. */
void LCD12864RSPI::writeByte(int data)
{
	digitalWrite(latchPin, HIGH);
	delayns();
	shiftOut(dataPin, clockPin, MSBFIRST, data);
	digitalWrite(latchPin, LOW);
}

/* Write command cmd into the screen. */
void LCD12864RSPI::writeCommand(int cmd)
{
	int H_data,L_data;
	H_data = cmd;
	H_data &= 0xf0;           //Mask lower 4 bit data
	L_data = cmd;             //Format: xxxx0000
	L_data &= 0x0f;           //Mask higher 4 bit data
	L_data <<= 4;             //Format: xxxx0000
	writeByte(0xf8);          //RS=0, an instruction is to be written.
	writeByte(H_data);
	writeByte(L_data);
}

/* Write data data into the screen. */
void LCD12864RSPI::writeData(int data)
{
	int H_data,L_data;
	H_data = data;
	H_data &= 0xf0;           //Mask lower 4 bit data
	L_data = data;            //Format: xxxx0000
	L_data &= 0x0f;           //Mask higher 4 bit data
	L_data <<= 4;             //Format: xxxx0000
	writeByte(0xfa);          //RS=1, data is to be written.
	writeByte(H_data);
	writeByte(L_data);
}

/* Initialize the screen. */
void LCD12864RSPI::initialise()
{
    pinMode(latchPin, OUTPUT);     
    pinMode(clockPin, OUTPUT);    
    pinMode(dataPin, OUTPUT);
    digitalWrite(latchPin, LOW);
    delayns();

    writeCommand(0x30);        //Function related command. 功能设定控制字
    writeCommand(0x0c);        //Display related command. 显示开关控制字
    writeCommand(0x01);        //Clear related command. 清除屏幕控制字
    writeCommand(0x06);        //Preset point instruction command. 进入设定点控制字
}

/* Clear the screen. */
void LCD12864RSPI::clear(void)
{  
    writeCommand(0x30);
    writeCommand(0x01); // Clear screen command.
}

/* Move cursor to 0-based column X and 0-based row Y as the starting point of the next display. */
void LCD12864RSPI::moveCursor(int X, int Y)
{
  switch(Y)
   {
     case 0:  X|=0x80;break;
     case 1:  X|=0x90;break;
     case 2:  X|=0x88;break;
     case 3:  X|=0x98;break;
     default: break;
   }
  writeCommand(X);
}

/* Display formated string at 0-based column X and 0-based row Y. The formatted string should not be longer than 64 characters. */
void LCD12864RSPI::printf(int X, int Y, char *fmt, ... ){
	char tmp[64]; // resulting string limited to 64 chars
	va_list args;
	va_start (args, fmt );
	int length = vsnprintf(tmp, 64, fmt, args);
	va_end (args);
	displayString(X, Y, (unsigned char *)tmp, length);
}

/* Clear the y-th row (y is 0-based) */
void LCD12864RSPI::clearRow(int Y)
{
  fillBlock(0, Y, ' ', 16);
}

/* Clear the block starting from 0-based column X and 0-based row Y. The cleared block is of len byte long. */
void LCD12864RSPI::clearBlock(int X, int Y, int len)
{
  fillBlock(X, Y, ' ', len);
}

/* Fill block starting from 0-based column X and 0-based row Y with the character c. The filled block is of length len.*/
void LCD12864RSPI::fillBlock(int X, int Y, uchar c, int len) 
{
  moveCursor(X, Y);
  int i;
  int y2 = Y;
  for(i=0; i<len; i++) {
    writeData(c);
    if(i>0 && i%15==0) {
        y2++;
        moveCursor(0, y2);
    }    
  }
}

/* Display string ptr (with length len) at 0-based column X and 0-based row Y. */
void LCD12864RSPI::displayString(int X,int Y, uchar *ptr,int len)
{
	int i;
	int y2 = Y;
	moveCursor(X, Y);    
	for(i=0; i<len; i++)
	{ 
	    writeData(ptr[i]);
		if(i>0 && i%15==0) {
			y2++;
			moveCursor(0, y2);
		}
	}
}

/* Display integer number at 0-based column X and 0-based row Y. */
void LCD12864RSPI::displayInteger(int X, int Y, int number)
{
	String txt = String(number);
	char buf[20];
	txt.toCharArray(buf, 20);
	LCDA.displayString(X, Y, (unsigned char*)buf, txt.length());
}

/* Display a single character sig at 0-based column X and 0-based row Y. */
void LCD12864RSPI::displaySig(int X,int Y,int sig)
{
	moveCursor(X, Y);
	writeData(sig);
}

/* Display the full screen using data from p. */
void LCD12864RSPI::drawFullScreen(uchar *p)
{
	int ygroup,x,y,i;
	int temp;
	int tmp;
             
	for(ygroup=0;ygroup<64;ygroup++)
	{                           
		if(ygroup<32)
		{
			x=0x80;
			y=ygroup+0x80;
		}
		else 
		{
			x=0x88;
			y=ygroup-32+0x80;    
		}         
		writeCommand(0x34);
		writeCommand(y);   
		writeCommand(x);
		writeCommand(0x30);  
		tmp=ygroup*16;
		for(i=0;i<16;i++)
		{
			temp=p[tmp++];
			writeData(temp);
		}
	}
	writeCommand(0x34);
	writeCommand(0x36);
}

/* Display image */
void LCD12864RSPI::drawImage1(uchar img[])
{
    unsigned int i;
    unsigned char page,column;  
    for(page=0xB0;page<0xB4;page++)  
    {  
        writeCommand(page);  //set page address   
        writeCommand(0x10);  //set Column address MSB   
        writeCommand(0x04);  //set column address LSB        
        i = (0xB3-page)*128;        
        for(column=0;column<128;column++)  
        {  
            writeData(~img[i+column]);  
        } 
    } 
    writeCommand(0x34);        //Write extension instruction
    writeCommand(0x36);        //Display image
}

/* Display image. */
void LCD12864RSPI::drawImage2(uchar img[])
{
    unsigned int i;
    unsigned char page,column;  

    for(page=0xB4;page<0xB8;page++)  
    {  
        writeCommand(page);  //set page address   
        writeCommand(0x10);  //set Column address MSB   
        writeCommand(0x04);  //set column address LSB         
        i = (0xB7-page)*128;        
        for(column=0;column<128;column++)  
        {  
            writeData(~img[i+column]);  
        } 
    }  
    writeCommand(0x34);        //Write extension instruction
    writeCommand(0x36);        //Display image
}



LCD12864RSPI LCDA = LCD12864RSPI();