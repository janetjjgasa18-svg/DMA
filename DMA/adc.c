#include "adc.h"

void ADC1_DMA_Init(void) {
    // 1. Habilitar reloj de GPIOA, AFIO y ADC1
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_ADC1EN;

    // 2. Configurar prescaler del ADC (PCLK2 = 72 MHz / 6 = 12 MHz, máx permitido 14 MHz)
    RCC->CFGR &= ~RCC_CFGR_ADCPRE;
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

    // 3. Configurar PA1 como entrada analógica (MODE1 = 00, CNF1 = 00)
    // Nota: Se usa PA1 asumiendo que PA0 lo tienes reservado para EXTI0
    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);

    // 4. Configurar secuencia y tiempo de muestreo para Canal 1 (PA1)
    ADC1->SQR1 &= ~ADC_SQR1_L;         // 1 conversión en la secuencia (L = 0000)
    ADC1->SQR3 = 1;                    // Primera conversión = Canal 1 (PA1)
    ADC1->SMPR2 |= ADC_SMPR2_SMP1;     // Tiempo de muestreo máximo (239.5 ciclos) para estabilidad

    // 5. Habilitar modo continuo (CONT), petición DMA (DMA) y encender ADC (ADON)
    ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_DMA | ADC_CR2_ADON;

    // Pequeño retardo de estabilización del ADC
    for (volatile int i = 0; i < 1000; i++);

    // 6. Calibración por hardware del ADC1
    ADC1->CR2 |= ADC_CR2_RSTCAL;
    while (ADC1->CR2 & ADC_CR2_RSTCAL);

    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL);

    // 7. Iniciar conversiones continuas (ADON por segunda vez o SWSTART)
    ADC1->CR2 |= ADC_CR2_ADON;
}