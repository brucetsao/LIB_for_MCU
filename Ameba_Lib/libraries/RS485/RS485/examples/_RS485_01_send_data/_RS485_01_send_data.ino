/*
 *  This sketch shows how to send data through RS-485 standard.
 *  This standard defines the electrical characteristics of drivers
 *  and receivers for use in digital systems. It does not specify
 *  or recommend any communications protocol. For a complete
 *  communication protocol, please see the Modbus examples.
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
 *  Implementation:   Ahmad Saad
 */

// Include always this library when you are using the RS-485 functions
#include <RS485.h>
//Include the SPI library
#include <SPI.h>
// Create an instance
RS485 myDevice = RS485();


void setup() {

  // Power on the USB for viewing data in the serial monitor
  Serial.begin(115200);
  delay(100);

  // Initializes the module and assigns the SPI
  if ( myDevice.begin() == 0) {
    Serial.println("RS-485 module started successfully");
  } else {
    Serial.println("RS-485 did not initialize correctly");
  }
  delay(100);

  // Configure the baud rate of the module
  myDevice.baudRateConfig(9600);
  // Configure the parity bit as disabled
  myDevice.parityBit(DISABLE);
  // Use one stop bit configuration
  myDevice.stopBitConfig(1);
  // Print hello message
  Serial.println("Hello this is RS-485 communication send data example.");
}



void loop() {
  // Reading the analog input 1
  int analog1 = analogRead(A1);
  //Reading the analog input 2
  int analog2 = analogRead(A2);

  // Send data through RS-485 line
  myDevice.send("Data from analog1 input : ");
  myDevice.send(analog1);
  myDevice.send("\n");

  myDevice.send("Data from analog2 input : ");
  myDevice.send(analog2);
  myDevice.send("\n");

  delay(1000);
}




