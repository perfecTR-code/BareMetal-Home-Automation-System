#include "SPI.h"

void Delay_us(uint32_t us) {
    SysTick->LOAD = (us * 16) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = (1 << 2) | (1 << 0);
    while (!(SysTick->CTRL & (1 << 16)));
    SysTick->CTRL = 0;
}

void SPI_Send_Data(uint8_t data) {
	//Wait until transmitter buffer is empty
    while (!(SPI2->SR & (1 << 1)));
    SPI2->DR = data;
    //Wait until transmition gets completed
    while (!(SPI2->SR & (1 << 1)));
}

uint8_t SPI_Receive_Data(void) {
	//Send dummy
    SPI_Send_Data(0xFF);
    while (!(SPI2->SR & (1 << 0)));
    //wait until it send the data completely
    //Read the data
    return (uint8_t)SPI2->DR;
}
