#include "dma.h"

void DMA1_ADC1_Init(volatile uint16_t *buffer, uint16_t longitud) {
    // 1. Habilitar el reloj para DMA1 en el bus AHB (Bit 0)
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. Asegurar que el Canal 1 esté deshabilitado antes de configurarlo
    DMA1_Channel1->CCR &= ~DMA_CCR1_EN;

    // 3. Configurar direcciones de origen (Periférico) y destino (Memoria)
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR); // Dirección fija del ADC1
    DMA1_Channel1->CMAR = (uint32_t)buffer;        // Dirección base del arreglo en RAM

    // 4. Número de datos a transferir antes de reiniciar el ciclo
    DMA1_Channel1->CNDTR = longitud;

    // 5. Configurar registro de control CCR del Canal 1:
    // - DIR = 0: Lectura desde periférico hacia memoria
    // - CIRC = 1: Modo circular (reinicia CNDTR y CMAR automáticamente)
    // - PINC = 0: Dirección de periférico fija
    // - MINC = 1: Incremento automático de dirección de memoria
    // - PSIZE = 01 (16 bits): ADC1->DR entrega 12 bits alineados en 16 bits
    // - MSIZE = 01 (16 bits): El arreglo en memoria es uint16_t
    // - PL = 10: Prioridad alta
    DMA1_Channel1->CCR = 0; // Limpiar configuración previa
    DMA1_Channel1->CCR |= DMA_CCR1_MINC   |  // Incremento de memoria
                          DMA_CCR1_CIRC   |  // Modo circular
                          DMA_CCR1_PSIZE_0|  // Tamaño periférico: 16 bits
                          DMA_CCR1_MSIZE_0|  // Tamaño memoria: 16 bits
                          DMA_CCR1_PL_1;     // Prioridad alta

    // 6. Habilitar el Canal 1 del DMA1
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
}