//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
// This program is used for Arduino V1.6.5 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, world!");
   lcd.setCursor(0,1);
  lcd.print("Power By BruceTsao!");
   lcd.setCursor(0,2);
  lcd.print("2Power By BruceTsao!");
   lcd.setCursor(0,3);
  lcd.print("3Power By BruceTsao!");
}


void loop()
{
}
