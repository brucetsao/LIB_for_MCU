/*
 *
 *  This sketch shows the use of the Modbus communication protocol over
 *  RS-485 standard, and the use of the main functions of the library.
 *. Modbus allows for communication between many devices connected
 *  to the same network. There are many variants of Modbus protocols,
 *  but Arduino implements the RTU format. Modbus RTU is the most
 *  common implementation available for Modbus.
 *
 *  This example shows the use of the Arduino RS-485 in slave mode.
 *
 *  Copyright (C) 2014 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version:          0.1
 *  Design:           David Gascon
 *  Implementation:   Ahmad Saad
 */

// Include these libraries for using the RS-485 and Modbus functions
#include <RS485.h>
#include <ModbusSlave485.h>
#include <SPI.h>

// Create new mbs instance
ModbusSlave485 mbs;

// Slave registers
enum {
  MB_0,   // Register 0
  MB_1,   // Register 1
  MB_2,   // Register 2
  MB_3,   // Register 3
  MB_4,   // Register 4
  MB_REGS // Dummy register. using 0 offset to keep size of array
};

int regs[MB_REGS];

void setup() {

  // Power on the USB for viewing data in the serial monitor
  Serial.begin(115200);

  // Modbus slave configuration parameters
  // SlaveId
  const unsigned char SLAVE = 1;
  // Baud rate
  const long BAUD = 9600;

  // Configure msb with config settings
  mbs.configure(SLAVE, BAUD);
}

void loop()
{
  // Pass current register values to mbs
  mbs.update(regs, MB_REGS);

  // Read all the analog Inputs, and store the values in
  // the Modbus registers.
  regs[MB_0] = analogRead(A1);  // Read input A1
  regs[MB_1] = analogRead(A2);  // Read input A2
  regs[MB_2] = analogRead(A3);  // Read input A3
  regs[MB_3] = analogRead(A4);  // Read input A4
  regs[MB_4] = analogRead(A5);  // Read input A5

  delay(10);
}
