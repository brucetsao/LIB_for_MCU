/*   
 *   
 *  This sketch shows the use of the Modbus communication protocol over 
 *  RS-485 standard, and the use of the main functions of the library.
 *. Modbus allows for communication between many devices connected
 *  to the same network. There are many variants of Modbus protocols, 
 *  but Arduino implements the RTU format. Modbus RTU is the most 
 *  common implementation available for Modbus. 
 *  
 *  This example shows how to create a network with several slaves, 
 *  and how to get data from them. 
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
#include <ModbusMaster485.h>
#include <SPI.h>
// Instantiate ModbusMaster object as slave ID 1
ModbusMaster485 temperatureSensor(1); 

// Instantiate ModbusMaster object as slave ID 2
ModbusMaster485 co2Sensor(2); 


// Define addresses for reading
#define tempRegister 10
#define co2Register  0

// Define the number of bits to read
#define bytesQty 1

void setup()
{
  // Power on the USB for viewing data in the serial monitor
  Serial.begin(115200);

  // Initialize Modbus communication baud rate
  temperatureSensor.begin(9600);
  co2Sensor.begin(9600);

  // Print hello message
  Serial.println("Modbus communication over RS-485");
  delay(100);
}


void loop()
{
//              Reading from the temperature sensor 
// *****************************************************************************************

  // This variable will store the result of the communication
  // result = 0 : no errors
  // result = 1 : error occurred 
  int result =  temperatureSensor.readInputRegisters(tempRegister,bytesQty);

  if (result != 0) {
    // If no response from the slave, print an error message
    Serial.println("Communication error. Couldn't read from temperature sensor");
    delay(100);
  } 
  else { 

    // If all OK
    Serial.print("Temperature read from slave ID 1: ");

    // Print the read data from the slave 
    Serial.print(temperatureSensor.getResponseBuffer(0));
    delay(100);
  }

  Serial.print("\n");
  delay(1000);


//              Reading from the co2 sensor          
// *****************************************************************************************

  // result = 0 : no errors
  // result = 1 : error occurred  
  result =  co2Sensor.readHoldingRegisters(co2Register,bytesQty);

  if (result != 0) {
    // If no response from the slave, print an error message 
    Serial.println("Communication error. Couldn't read from the C02 sensor");
    delay(100);
  } 
  else { 

    // If all OK
    Serial.print("CO2 data read from slave ID 2: ");

    // Print the read data from the slave 
    Serial.print(co2Sensor.getResponseBuffer(0));
    delay(100);
  }

  Serial.print("\n");
  delay(1000);
  
  // Clear the response buffer 
  temperatureSensor.clearResponseBuffer();
  co2Sensor.clearResponseBuffer();

}




