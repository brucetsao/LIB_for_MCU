

#include "AmebaWS2812.h"
#include "rtl8195a.h"
#include "FreeRTOS.h"
#include "task.h"
#if defined(__AVR__)
#include <util/delay.h>
#endif

AmebaWS2812::AmebaWS2812() {
}

boolean AmebaWS2812::begin(int pinNo, int LedNo) {
 	static int blinking = 1;
	//	setPin(pinNo)  ;
//	setPixel(LedNo) ;
  gpio_init(&gpio_ws2811, GPIO_WS2811_PIN);
  gpio_dir(&gpio_ws2811, PIN_OUTPUT);
  gpio_mode(&gpio_ws2811, PullNone);
  //IsBlink =1;
  IsMeteor =1;
  return true;
}
/*
boolean AmebaWS2812::begin(PinName pins) {
 	static int blinking = 0;
	sendPin(pins) ;
  gpio_init(&gpio_ws2811, GPIO_WS2811_PIN);
  gpio_dir(&gpio_ws2811, PIN_OUTPUT);
  gpio_mode(&gpio_ws2811, PullNone);
  //IsBlink =1;
  IsMeteor =1;
  return true;
}
*/
void AmebaWS2812::show(void) {
	  static int i = 0;

    static int meteorIdx = 0;

    int meteor_length = 8;
    int meteor_head_idx = 0;
    int meteor_tail_idx = meteor_head_idx - (meteor_length - 1);

    static int blinking = 0;
    extern uint32_t xTaskGetTickCount();
    
    portTickType xLastWakeTime = xTaskGetTickCount(); 
         
    for( ;; ) { 
				Config_r =_r;
				Config_g =_g;
				Config_b =_b;
        if (IsBlink) {
            if (blinking) {
                setColor(Config_r, Config_g, Config_b);
                sendLEDs();
            } else {
                setColor(0, 0, 0);
                sendLEDs();
            }
            blinking ^= 1;
        } else if (IsMeteor) {

            taskENTER_CRITICAL(); 

            for (i = 0; i < NUM_LEDS; ++ i) {

              if ( (i <= meteor_head_idx) && (i >= meteor_tail_idx)) {
                  sendByte(Config_g - (meteor_head_idx - i) * (Config_g /meteor_length));
                  sendByte(Config_r - (meteor_head_idx - i) * (Config_r /meteor_length));
                  sendByte(Config_b - (meteor_head_idx - i) * (Config_b /meteor_length));
              } else {
                  sendByte(BackgroudColor[i][0]);
                  sendByte(BackgroudColor[i][1]);
                  sendByte(BackgroudColor[i][2]);
              }

            }

            taskEXIT_CRITICAL(); 

            //meteorIdx = (meteorIdx + 1) % NUM_LEDS;
            meteor_head_idx = (meteor_head_idx + 1) % (NUM_LEDS + (meteor_length - 1));
            meteor_tail_idx = meteor_head_idx - (meteor_length - 1);

        }


        vTaskDelayUntil( &xLastWakeTime, ( Blink_ms / portTICK_RATE_MS ) ); 
    } 

}

void AmebaWS2812::sendByte(uint8_t b){
	
	uint8_t i = 8;

    do {
        if ( (b & 0x80) == 0) { /* send a '0', T0H + T0L */
            /* T0H, 0 code, high voltage time, 0.4us +- 150ns */
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            /* T0L, 0 code, low voltage time, 0.85us +-150ns */
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
        } else { /* send a '1', T1H + T1L */
            /* T1H, 1 code, high voltage time, 0.8us +- 150ns */
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000001); // 96ns
            /* T1L, 1 code, low voltage time, 0.45us +-150ns */
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
            HAL_WRITE32(GPIO_REG_BASE, GPIO_PORTA_DR, 0x00000000); // 96ns
        }
        b = b + b;
    } while (-- i != 0);	
	
}

void AmebaWS2812::send() {
    sendByte(_g);
    sendByte(_r);
    sendByte(_b);
}  

void AmebaWS2812::setColor(uint8_t r, uint8_t g, uint8_t b) {
    printf("%s: (r,g,b) = (%d,%d,%d)\n", __FUNCTION__, r,g,b);

    _r = r;
    _g = g;
    _b = b;
} 

void AmebaWS2812::sendLEDs() {
    uint8_t i;
    taskENTER_CRITICAL(); 
    for (i = 0; i < NUM_LEDS; ++ i) {
      send();
    }
    taskEXIT_CRITICAL(); 
}

void AmebaWS2812::setPin(int pin){
	/*
		switch(pin)
		{
			case 0:
			GPIO_WS2811_PIN = PA_6	;
			break ;
			case 1:
			GPIO_WS2811_PIN = PA_7	;
			break ;
			case 2:
			GPIO_WS2811_PIN = PA_5	;
			break ;
			case 3:
			GPIO_WS2811_PIN = PD_4	;
			break ;
			case 4:
			GPIO_WS2811_PIN = PD_5	;
			break ;
			case 5:
			GPIO_WS2811_PIN = PA_4	;
			break ;
			case 6:
			GPIO_WS2811_PIN = PA_3	;
			break ;
			case 7:
			GPIO_WS2811_PIN = PA_2	;
			break ;
			case 8:
			GPIO_WS2811_PIN = PB_4	;
			break ;
			case 9:
			GPIO_WS2811_PIN = PB_5	;
			break ;
			case 10:
			GPIO_WS2811_PIN = PC_0	;
			break ;
			case 11:
			GPIO_WS2811_PIN = PC_2	;
			break ;
			case 12:
			GPIO_WS2811_PIN = PC_3	;
			break ;
			case 13:
			GPIO_WS2811_PIN = PC_1	;
			break ;
			case 14:
			GPIO_WS2811_PIN = PB_3	;
			break ;
			case 15:
			GPIO_WS2811_PIN = PB_2	;
			break ;
			case 16:
			GPIO_WS2811_PIN = PA_1	;
			break ;
			case 17:
			GPIO_WS2811_PIN = PA_0	;
			break ;
			
		}
		*/
	}

void AmebaWS2812::setPixel(int no){
	  NUM_Pixels = no ;
}

/*********************************************************************/
