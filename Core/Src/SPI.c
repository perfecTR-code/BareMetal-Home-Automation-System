#include "stm32f4xx.h"


void SPI_Send_Data(uint8_t data) {
	//Wait until transmitter transmitter buffer is empty
    while (!(SPI2->SR & (1 << 1)));
    SPI2->DR = data;
}

uint8_t SPI_Receive_Data(void) {
	//Send dummy
    SPI_Send_Data(0xFF);
    while (!(SPI2->SR & (1 << 0)));
    //wait until it send the data completely
    //Read the data
    return (uint8_t)SPI2->DR;
}
void SPI_Init(void){
	//Enable SPI 2 clock
	RCC->APB1ENR |= (1u << 14);
	//Enable A port
	RCC->AHB1ENR |= (1u << 0);
	// Open GPIOB and GPIOC
	RCC->AHB1ENR |= (1 << 1) | (1 << 2);
}
/*
 *********************************************************************
							     PINOUT
	//CSN -> PC_8
	//MOSI -> PB_15
	//MISO -> PB14
	//PB_13 -> SCLK
	//PB_2 -> CN

 *********************************************************************
*/

void SPI_Config(void){
	//Baud rate 1/8
	SPI2->CR1 &= ~(7 << 3);
	SPI2->CR1 |= (2 << 3);
	//Peripheral clock enable
	SPI2->CR1 |= (1 << 6);
	// Connect the MOSI,MISO,SCLK pins to SPI unit
	GPIOB->AFR[1] &= ~((15 << 20) | (15 << 24) | (15 << 28));
	GPIOB->AFR[1] |=  ((5 << 20)  | (5 << 24)  | (5 << 28));

	// PB13, PB14, PB15 pins -> Alternate Function mode
	GPIOB->MODER &= ~((3 << 26) | (3 << 28) | (3 << 30));
	GPIOB->MODER |=  ((2 << 26) | (2 << 28) | (2 << 30));

	// Set the upper pins to high speed mode
	GPIOB->OSPEEDR |= ((3 << 26) | ((3 << 28)) | (3 << 30));

	// PC8 (CSN) pin -> output mode:
	GPIOC->MODER &= ~(3 << 16);
	GPIOC->MODER |=  (1 << 16);

	// PB2 (CE) pin -> output mode:
	GPIOB->MODER &= ~(3 << 4);
	GPIOB->MODER |=  (1 << 4);
}
