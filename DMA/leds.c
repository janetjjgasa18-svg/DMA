#include "leds.h"

void LED_PC13_Init(void) {
    // 1. Habilitar reloj para el Puerto C en el bus APB2
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // 2. Configurar PC13 como salida Push-Pull a 2 MHz (MODE13 = 10, CNF13 = 00)
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_1;

    // 3. Iniciar con el LED apagado (en la Blue Pill PC13 es activo en bajo)
    GPIOC->BSRR = GPIO_BSRR_BS13;
}

void LED_PC13_On(void) {
    GPIOC->BRR = GPIO_BRR_BR13; // Pone PC13 en 0V (enciende)
}

void LED_PC13_Off(void) {
    GPIOC->BSRR = GPIO_BSRR_BS13; // Pone PC13 en 3.3V (apaga)
}

void LED_PC13_Toggle(void) {
    GPIOC->ODR ^= GPIO_ODR_ODR13; // Invierte el estado actual
}