#include "stm32f10x.h"
#include <stdio.h>
#include "leds.h"
#include "exti.h"
#include "USART.h"
#include "delay.h"
#include "adc.h"
#include "dma.h"

#define BUFFER_SIZE 10

// Buffer en RAM donde el DMA depositará automáticamente las lecturas del CAD
volatile uint16_t adc_buffer[BUFFER_SIZE];

int main(void) {
    char tx_buffer[64];
    uint32_t suma_adc = 0;
    uint16_t promedio_adc = 0;
    float voltaje = 0.0f;

    // 1. Inicialización de periféricos base del proyecto
    LED_PC13_Init();      // LED integrado en PC13
    init_exti();         // Interrupción externa en PA0
    init_usart1();    // Comunicación serial por PA9 (TX) / PA10 (RX)
    Delay_Init();         // Temporizador para retardos

    // 2. Inicializar DMA1 Canal 1 apuntando al arreglo en memoria
    // Es fundamental inicializar el DMA antes de arrancar el ADC
    DMA1_ADC1_Init(adc_buffer, BUFFER_SIZE);

    // 3. Inicializar ADC1 en modo continuo con disparos hacia el DMA
    ADC1_DMA_Init();

    usart1_sendstring("Sistema iniciado: Adquisicion ADC por DMA activa.\r\n");

    while (1) {
        // Calcular el promedio de las muestras almacenadas en memoria por el DMA
        suma_adc = 0;
        for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
            suma_adc += adc_buffer[i];
        }
        promedio_adc = (uint16_t)(suma_adc / BUFFER_SIZE);

        // Convertir valor digital (12 bits: 0 - 4095) a voltaje (0.00V - 3.30V)
        voltaje = (promedio_adc * 3.3f) / 4095.0f;

        // Formatear y transmitir por USART1
        sprintf(tx_buffer, "ADC (DMA): %4u | Voltaje: %.2f V\r\n", promedio_adc, voltaje);
        usart1_sendstring(tx_buffer);

        // Parpadeo de testigo en PC13 cada 500 ms
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        Delay_ms(500);
    }
}