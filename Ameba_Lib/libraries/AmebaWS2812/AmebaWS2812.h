
#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif			
#include "gpio_api.h"

#define GPIO_WS2811_PIN     PA_0
#define NUM_LEDS            16


class AmebaWS2812 {
 public:
  AmebaWS2812();
  boolean begin(int pinNo, int LedNo);
  void show(void);
  void sendByte(uint8_t b);
  void send();
  void setColor(uint8_t r, uint8_t g, uint8_t b);
  void sendLEDs();
  void setPin(int pin);
  void setPixel(int no) ;

 private: 	
 //	PinName GPIO_WS2811_PIN = PA_1;
	int NUM_Pixels  ; 
	uint8_t _r, _g, _b;
	gpio_t gpio_ws2811;
	uint8_t IsBlink = 0;
	uint8_t IsMeteor = 0;
	uint8_t Config_r=0, Config_g=0, Config_b=0;	
	uint8_t Blink_ms = 500;
	uint8_t BackgroudColor[NUM_LEDS][3] = {
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},/* {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
    {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, */
	};

};

