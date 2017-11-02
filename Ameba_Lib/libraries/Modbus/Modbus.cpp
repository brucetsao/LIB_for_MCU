/*
 * Modbus.cpp
 */

#include "modbus.h"

#define DEBUG_LEVEL   0

void ModbusMessage(int code)
{
   Serial.print("Moubus Code : ");
    switch(code) {
        case Modbus_Undefined: 
                Serial.println(F("Undefined")); break;
        case Modbus_OK: 
                Serial.println(F("OK")); break;
        case Modbus_Wait:
                Serial.println(F("Wait")); break;
        case Modbus_Response:
                Serial.println(F("Response")); break;
        case Modbus_Timeout:
                Serial.println(F("Timeout")); break;
        case Modbus_BadCRC:
                Serial.println(F("Bad CRC")); break;
        default:        
                Serial.println(F("bad ErrCode")); break;
    }
}

void Modbus::begin(SoftwareSerial* serialPort, uint32_t baud)
{
    ModbusPort = serialPort;
    baudRate = baud;
    (*ModbusPort).begin(baud);
}

void Modbus::getCRC(uint8_t* frame, int frameLength)
{
    uint16_t CRC = 0xFFFF;
    char CRCLSB;

    highCRC = lowCRC = 0xFF;
    for (int i = 0; i < frameLength-2; i++)
    {
        CRC = (CRC ^ frame[i]);
        for (int j = 0; j < 8; j++)
        {
            lowCRC = (uint8_t)(CRC & 0x0001);
            CRC = (CRC >> 1) & 0x7FFF;
            if (lowCRC==1)
               CRC ^= 0xA001;
        }
    }
    highCRC = highByte(CRC);
    lowCRC = lowByte(CRC);
}


uint8_t Modbus::getCRCHighbyte()
{
		return (uint8_t)highCRC ;
}

uint8_t Modbus::getCRCLowbyte()
{
		return (uint8_t)lowCRC ;
}

void Modbus::fillCRC(uint8_t* frame, int pos)
{
	frame[pos] = getCRCLowbyte() ;
	frame[pos+1] = getCRCHighbyte() ;
}
void Modbus::clearBus()
{
    while ((*ModbusPort).available()>0) {
        (*ModbusPort).read();
        delay(1);
    }
}
/*
void Modbus::dumpHex(byte buffer[], int length)
{
    for (int i=0; i<length; i++) {
        if (buffer[i]<16) (*ModbusPort).print("0");
        (*ModbusPort).print(buffer[i], HEX);
        (*ModbusPort).print(" ");
        if (i%32==31)  (*ModbusPort).print("\n       ");
    }  
    (*ModbusPort).println();
}
*/

