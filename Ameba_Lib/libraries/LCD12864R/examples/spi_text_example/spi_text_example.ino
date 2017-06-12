/*
LCD  Arduino
PIN1 = GND
PIN2 = 5V
RS(CS) = 8; 
RW(SID)= 9; 
EN(CLK) = 3;
PIN15 PSB = GND;
*/

#include "LCD12864RSPI.h"

void setup()
{
  LCDA.initialise();
  delay(100);
}

int i = 1;
void loop()
{
  LCDA.clear();
  delay(10);
  LCDA.printf(0, 0, "This is an integer %d, which will be incremented.", i);
  delay(1000);
  i++;
}

