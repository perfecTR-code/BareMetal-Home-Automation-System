#include "stm32f4xx.h"


void SPI_Send_Data(uint8_t data) {
	//Wait until transmitter transmitter buffer is empty
    while (!(SPI1->SR & (1 << 1)));
    SPI1->DR = data;
}

uint8_t SPI_Receive_Data(void) {
	//Send dummy
    SPI_Send_Data(0xFF);
    while (!(SPI1->SR & (1 << 0)));
    //wait until it send the data completely
    //Read the data
    return (uint8_t)SPI1->DR;
}
void SPI_Init(void){
	//Enable SPI 2 clock
	RCC->APB1ENR |= (1u << 14);
	//Enable A port
	RCC->AHB1ENR |= (1u << 0);
}




void SPI_Config(void){
	//Baud rate 1/8
	SPI1->CR1 &= ~(7 << 3);
	SPI1->CR1 |= (2 << 3);
	//Peripheral clock enable
	SPI2->CR1 |= (1 << 6);

}

