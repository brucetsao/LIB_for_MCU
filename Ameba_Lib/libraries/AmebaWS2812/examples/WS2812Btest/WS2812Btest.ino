#include "AmebaWS2812.h"

AmebaWS2812 ledstrip = ws2812b();

void setup() {
  Serial.begin(9600);
  Serial.println("ws2812b test");
  ledstrip.begin();
  ledstrip.setColor(0,128,255);
  ledstrip.show();
}


void loop() {
  
  delay(500);
}
