#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3ad.h>

boolean play_state = HIGH;  // module play state HIGH:NOT playing; LOW:playing
boolean adState = HIGH;     // is the AD music playing? HIGH:NOT playing; LOW:playing
int playStatePin = 5;       //digital pin5: read the module play state

SoftwareSerial mySerial(10, 11); // RX, TX (Serial wiring; RX-TX, TX-RX)

void setup () {
  Serial.begin (9600);
  mySerial.begin (9600);
  mp3_set_serial (mySerial);  //set softwareSerial for DFPlayer-mini mp3 module
  mp3_set_volume (14);        //value 0~30
  delay(1000);                //wait for the module setting finished
  attachInterrupt(0, playAd, RISING );     //connect a button on Arduino interrupt 0
          //check interrupt map https://www.arduino.cc/en/Reference/AttachInterrupt
}

// The delays are indispensible!!!
void loop () {
  delay(1000);  
  play_state = digitalRead(playStatePin);
  if (play_state == HIGH ) {
    mp3_reset ();
    delay(1000);
    mp3_next ();
    delay(100);
  }

  if (adState == LOW) {
    delay(100);
    play_state = digitalRead(playStatePin);
    while (play_state == LOW)
      play_state = digitalRead(playStatePin);
    adState = HIGH;
    ad_stop();
    delay(100);
  }
}

void playAd() {
  ad_play (2);
  adState = LOW;
}
