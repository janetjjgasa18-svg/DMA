#include "delay.h"

void Delay_Init(void) {
    // Asegurar que SysTick use el reloj del procesador (AHB = 72 MHz)
    // y deshabilitarlo mientras no esté en uso
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;
}

void Delay_ms(uint32_t ms) {
    // Para un reloj del sistema (SYSCLK) de 72 MHz:
    // 1 ms = 72,000 ciclos de reloj
    SysTick->LOAD = 72000 - 1;
    SysTick->VAL  = 0;
    
    // Habilitar SysTick (Bit 0 = 1) con fuente de reloj del procesador (Bit 2 = 1)
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;

    for (uint32_t i = 0; i < ms; i++) {
        // Esperar hasta que la bandera COUNTFLAG (Bit 16) se ponga en 1
        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    }

    // Apagar SysTick al terminar
    SysTick->CTRL = 0;
}

void Delay_us(uint32_t us) {
    // Para un reloj del sistema (SYSCLK) de 72 MHz:
    // 1 us = 72 ciclos de reloj
    SysTick->LOAD = 72 - 1;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;

    for (uint32_t i = 0; i < us; i++) {
        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    }

    SysTick->CTRL = 0;
}