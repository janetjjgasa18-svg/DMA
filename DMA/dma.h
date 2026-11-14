#ifndef _dma
#define _dma

#include "stm32f10x.h"
#include <stdint.h>

// Inicializa DMA1 Canal 1 para transferir desde ADC1->DR a un buffer en RAM
void DMA1_ADC1_Init(volatile uint16_t *buffer, uint16_t longitud);

#endif // DMA_H