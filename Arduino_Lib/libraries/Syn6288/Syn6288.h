#ifndef Syn6288_h
#define Syn6288_h
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stddef.h>

#define HEADLEN      5
//#define LEN_OFFSET    2

//#define uint8_t    unsigned int
class Syn6288
{
  public:

//private:
uint8_t music;
uint8_t TEXTLEN;
uint8_t pi;
void play(uint8_t *text,uint8_t TEXTLEN,uint8_t music);
//void play(uint8_t *text,uint8_t music);
void Slaveboudset(uint16_t boudr);
void stop();
void restore();
void inquire();
void Pause();
void sleep();

};
#endif
