#ifndef _leds
#define _leds

#include "stm32f10x.h"

void LED_PC13_Init(void);
void LED_PC13_On(void);
void LED_PC13_Off(void);
void LED_PC13_Toggle(void);

#endif // LEDS_H