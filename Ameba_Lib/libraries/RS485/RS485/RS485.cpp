/*! \file RS485.cpp
    \brief Library for managing RS485 modules
    
    Copyright (C) 2009 Libelium Comunicaciones Distribuidas S.L.
    http://www.libelium.com
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.
   
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
  
    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
    Version:		0.1    
    Implementation:	Ahmad Saad
 */

//**********************************************************************
// Includes
//**********************************************************************

#include "RS485.h" 
#include "Arduino.h"
#include "../SPI/SPI.h"

//**********************************************************************
// Class contructors
//**********************************************************************

RS485::RS485()	{}


//**********************************************************************
// Public functions. 
//**********************************************************************
 
//!*************************************************************
//!	Name:	begin()													
//!	Description: Powers the 485 module and opens the SPI
//!	Param : void														
//!	Returns: "0" if no error, "1" if error							
//!*************************************************************
uint8_t RS485::begin()
{
	// Deselect the MAX chip
	pinMode(MAX_SS, OUTPUT);
	digitalWrite(MAX_SS, HIGH);
	delay(100);
	//Configure the MISO, MOSI, CS, SPCR.
	SPI.begin(); 
	//Set Most significant bit first
	SPI.setBitOrder(MSBFIRST);
	//Divide the clock frequency 
	SPI.setClockDivider(SPI_CLOCK_DIV32);
	//Set data mode 
	SPI.setDataMode(SPI_MODE0);
	delay(10);
	//Configure 8-bits comunnication mode
	maxWrite(LCR_REG , 0x03);
	//Disables clock pre dividier.
	maxWrite(PLLCFG_REG , 0x01);

	//Return 0 if all success. Else return 1. 
	if (maxRead(REVID_REG) == 0xA1)
		return 0;
	else
		return 1;
}

//!*************************************************************
//!	Name:	read(void)													
//!	Description: Receives data through the SPI.
//!	Param : void														
//!	Returns: uint8_t : data read					
//!*************************************************************
uint8_t RS485::read(void)
{
	return maxRead(RHR_REG);
}

//!*************************************************************
//!	Name:	send(uint8_t n)											
//!	Description: Sends data through the SPI.
//!	Param : uint8_t n: data to send.								
//!	Returns: void
//!*************************************************************
void RS485::send(uint8_t n)
{
	maxWrite(THR_REG, n);
}

//!*************************************************************
//!	Name:	send(int n)												
//!	Description: Sends data through the SPI.
//!	Param : int n: data to send.								
//!	Returns: void
//!*************************************************************
void RS485::send(int n)
{	
	if (n < 0) {
		maxWrite(THR_REG, '-');
		n = -n;
	}
	printIntegerInBase(n , 10);	
}

//!*************************************************************
//!	Name:	send(unsigned int n)										
//!	Description: Sends data through the SPI.
//!	Param : unsigned int n: data to send.							
//!	Returns: void
//!*************************************************************
void RS485::send(unsigned int n) 
{
	printIntegerInBase(n , 10);
}

//!*************************************************************
//!	Name:	send(long n)												
//!	Description: Sends data through the SPI.
//!	Param : long n: data to send.								
//!	Returns: void
//!*************************************************************
void RS485::send( long n) 
{
	if (n < 0) {
		maxWrite(THR_REG, '-');
		n = -n;
	}
	printIntegerInBase(n , 10);
}

//!*************************************************************
//!	Name:	send(unsigned long n)									
//!	Description: Sends data through the SPI.
//!	Param : unsigned long n: data to send.							
//!	Returns: void
//!*************************************************************
	void RS485::send(unsigned long n) 
	{
		printIntegerInBase(n , 10);
	}	
	
//!*************************************************************
//!	Name:	send(long n, uint8_t base)								
//!	Description: Sends data through the SPI.
//!	Param 	: long n: data to send.
//!			: uint8_t base : base representation. 
//!	Returns: void
//!*************************************************************
void RS485::send(long n, uint8_t base)
{
	if (base == BYTE) {
		maxWrite(THR_REG,(char) n);		
	} else
		printIntegerInBase(n, base);
	}

//!*************************************************************
//!	Name:	send(const char *s)
//!	Description: Sends data through the SPI.
//!	Param : const char s: the string to send 
//!	Returns: void
//!*************************************************************
void RS485::send (const char *s)
{
	while (*s) {
		maxWrite(THR_REG, *s++);
	}
} 


void RS485::send(double n) 
{
	printFloat(n, 10);
}

//!*************************************************************
//!	Name:	baudRateConfig(unsigned long speed)
//!	Description: It sets the speed of communication.
//!	Param: unsigned long speed: The baud-rate parameter.
//!	Returns: void
//!*************************************************************
void RS485::baudRateConfig(unsigned long speed)
{
	//!Configure the speed communication at 460800 bps.
	if (speed == 460800) {
		maxWrite(MODE2_REG, 0x80);
		maxWrite(HDPIXDELAY_REG, 0x0F);
		maxWrite(MODE1_REG,0x10);
		maxWrite(CLKSRC_REG,0x1A);
		maxWrite(BRGCFG_REG, 0x20);
		maxWrite(BRGDIVLSB_REG, 0x02);
		maxWrite(BRGDIVMSB_REG, 0x00);

	//!Configure the speed communication at 230400 bps.
	} else if (speed == 230400) {
		maxWrite(MODE2_REG, 0x80);
		maxWrite(HDPIXDELAY_REG, 0x0F);
		maxWrite(MODE1_REG,0x10);
		maxWrite(CLKSRC_REG,0x1A);
		maxWrite(BRGCFG_REG, 0x10);
		maxWrite(BRGDIVLSB_REG, 0x02);
		maxWrite(BRGDIVMSB_REG, 0x00);
		
	//!Configure the speed communication at 115200 bps.
	} else if (speed == 115200) {
		maxWrite(MODE2_REG, 0x80);
		maxWrite(HDPIXDELAY_REG, 0x0F);
		maxWrite(MODE1_REG,0x10);
		maxWrite(CLKSRC_REG,0x1A);
		maxWrite(BRGCFG_REG, 0x00);
		maxWrite(BRGDIVLSB_REG, 0x02);
		maxWrite(BRGDIVMSB_REG, 0x00); 

	//!Configure the speed communication at 57600 bps.
	} else if (speed == 57600) {		
		maxWrite(MODE2_REG, 0x80);
		maxWrite(HDPIXDELAY_REG, 0x0F);
		maxWrite(MODE1_REG,0x10);
		maxWrite(CLKSRC_REG,0x1A);
		maxWrite(BRGCFG_REG, 0x10);
		maxWrite(BRGDIVLSB_REG, 0x08);
		maxWrite(BRGDIVMSB_REG, 0x00);
			
	//!Configure the speed communication at 38400 bps.
	} else if (speed == 38400) {
		maxWrite(MODE2_REG, 0x80);
		maxWrite(HDPIXDELAY_REG, 0x0F);
		maxWrite(MODE1_REG,0x10);
		maxWrite(CLKSRC_REG,0x1A);
		maxWrite(BRGCFG_REG, 0x00);
		maxWrite(BRGDIVLSB_REG, 0x06);
		maxWrite(BRGDIVMSB_REG, 0x00);
		
	//!Configure the speed communication at 19200 bps.
	} else if (speed == 19200) {
		maxWrite(MODE2_REG, 0x80);
		maxWrite(HDPIXDELAY_REG, 0x0F);
		maxWrite(MODE1_REG,0x10);
		maxWrite(CLKSRC_REG,0x1A);
		maxWrite(BRGCFG_REG, 0x00);
		maxWrite(BRGDIVLSB_REG, 0x0C);
		maxWrite(BRGDIVMSB_REG, 0x00);
		
	//!Configure the speed communication at 9600 bps
	}  else if (speed == 9600) {		
		maxWrite(MODE2_REG, 0x80);
		maxWrite(HDPIXDELAY_REG, 0x0F);				
		maxWrite(MODE1_REG,0x10);		
		maxWrite(CLKSRC_REG,0x1A);
		maxWrite(BRGCFG_REG, 0x00);
		maxWrite(BRGDIVLSB_REG, 0x18);
		maxWrite(BRGDIVMSB_REG, 0x00);

	//!Configure the speed communication at 4800 bps
	}  else if (speed == 4800) {		
		maxWrite(MODE2_REG, 0x80);
		maxWrite(HDPIXDELAY_REG, 0x0F);				
		maxWrite(MODE1_REG,0x10);		
		maxWrite(CLKSRC_REG,0x1A);
		maxWrite(BRGCFG_REG, 0x00);
		maxWrite(BRGDIVLSB_REG, 0x30);
		maxWrite(BRGDIVMSB_REG, 0x00);

	//!Configure the speed communication at 4800 bps	
	}	else if (speed == 2400) {		
		maxWrite(MODE2_REG, 0x80);
		maxWrite(HDPIXDELAY_REG, 0x0F);				
		maxWrite(MODE1_REG,0x10);		
		maxWrite(CLKSRC_REG,0x1A);
		maxWrite(BRGCFG_REG, 0x00);
		maxWrite(BRGDIVLSB_REG, 0x60);
		maxWrite(BRGDIVMSB_REG, 0x00);
		
	//!Configure the speed communication at 1200 bps		
	}	else  {		
		maxWrite(MODE2_REG, 0x80);
		maxWrite(HDPIXDELAY_REG, 0x0F);				
		maxWrite(MODE1_REG,0x10);		
		maxWrite(CLKSRC_REG,0x1A);
		maxWrite(BRGCFG_REG, 0x00);
		maxWrite(BRGDIVLSB_REG, 0xC0);
		maxWrite(BRGDIVMSB_REG, 0x00);
	}
}

//!*************************************************************
//!	Name:	parityBit(bool state)
//!	Description: Enables or disables the parity bit.
//!	Param: bool state (ENABLED OR DISABLED).
//!	Returns: void
//!*************************************************************
void RS485::parityBit(bool state)
{
	if (state == ENABLE) {
		maxWrite(LCR_REG, maxRead(LCR_REG) | 0x0B) ;
	} else { 
		maxWrite(LCR_REG, maxRead(LCR_REG) & (0xF7));
	}
}

//!*************************************************************
//!	Name:	stopBitConfig(uint8_t numStopBits)
//!	Description: Selects the number of stop bits to be inserted.
//!	Param: uint8_t numStopBits state the number of stop bits.
//!	Returns: void
//!*************************************************************
void RS485::stopBitConfig(uint8_t numStopBits)
{
	if (numStopBits == 1) {
		maxWrite(LCR_REG, (maxRead(LCR_REG) & 0xFB));
	} else {
		maxWrite(LCR_REG, (maxRead(LCR_REG) | 0x04));
	}
}

//!*************************************************************
//!	Name:	reset(void) 
//!	Description: All register are reset and all FIFOs cleared.
//!	Param: void.
//!	Returns: bool, 1 if error, 0 if success.
//!*************************************************************
bool RS485::reset(void) 
{
	maxWrite(MODE2_REG, maxRead(MODE2_REG) | 0x01);
	maxWrite(MODE2_REG, maxRead(MODE2_REG) &  0xFE);

	//Configure 8-bits comunnication mode
	maxWrite(LCR_REG , 0x03);
	//Disables clock pre divider.
	maxWrite(PLLCFG_REG , 0x01);

	if (maxRead(REVID_REG) == 0xA1) {
		return 0;
	} else {
		return 1;
	}
}

//!*************************************************************
//!	Name:	flush(void) 
//!	Description: Clear both the receive and transmit FIFOs
//!	Param: void.
//!	Returns: void.
//!*************************************************************
void RS485::flush(void)
{
	maxWrite(MODE2_REG,0x02);
	maxWrite(MODE2_REG,0x80);
}


//!*************************************************************
//!	Name:	available(void)  
//!	Description:  Returns the number of bytes avaliable
//!	Param: void.
//!	Returns: uint8_t : number of bytes available.
//!*************************************************************
uint8_t RS485::available(void) 	
{	
	return  maxRead(RXFIFOLVL_REG);
}

//**********************************************************************
// Private functions
//**********************************************************************
 
//!*************************************************************
//!	Name:	maxWrite(char address, char data)
//!	Description: Write data in the MAX3107 adress.
//!	Param: char address, char data.
//!	Returns: void.
//!************************************************************* 
void RS485::maxWrite(char address, char data)
{	
	digitalWrite(MAX_SS,LOW);
	SPI.transfer(0x80 | address);
	SPI.transfer(data);    
	digitalWrite(MAX_SS,HIGH);
}


//!*************************************************************
//!	Name:	maxRead(char address) 
//!	Description: Read a data from the MAX3107.
//!	Param: char address
//!	Returns: uint8_t : data read.
//!*************************************************************
uint8_t RS485::maxRead(char address) 
{
	digitalWrite(MAX_SS,LOW);
	SPI.transfer(address);
	uint8_t data = SPI.transfer(0x00);
	digitalWrite(MAX_SS,HIGH);
	return data;
}


//!*************************************************************
//!	Name:	printIntegerInBase(unsigned long n , uint8_t base) 
//!	Description: It print a number n in a given base
//!	Param: unsigned long n , uint8_t base
//!	Returns: void.
//!*************************************************************
 void RS485::printIntegerInBase(unsigned long n , uint8_t base) 
 { 
	char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	unsigned long i = 0;

	if (n == 0) {
		maxWrite(THR_REG,'0');
		return;
	}
	while (n > 0) {
		buf[i++] = n % base;
		n /= base;
	}

	for (; i > 0; i--)
		maxWrite(THR_REG, buf[i - 1] < 10 ?
				'0' + buf[i - 1] :
				'A' + buf[i - 1] - 10);
}

void RS485::printFloat(double number, uint8_t digits) 
{

	// Handle negative numbers
	if (number < 0.0)
	{
		send('-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i=0; i<digits; ++i)
		rounding /= 10.0;
  
	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long int_part = (unsigned long)number;
	double remainder = number - (double)int_part;
	send(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0)
		send("."); 

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		int toPrint = int(remainder);
		send(toPrint);
		remainder -= toPrint; 
	}

}
