#include "SPI_CFG.h"

//Enable SPI 2 clock, Enable GPIO ports
void SPI_Clock_Init(void) {
	//Enable SPI 2 clock
	RCC->APB1ENR |= (1u << 14);
	//Enable A port
	RCC->AHB1ENR |= (1u << 0);
	// Open GPIOB and GPIOC
	RCC->AHB1ENR |= (1 << 1) | (1 << 2);
}

//Make our stm32 master, set baud rate, enable SSM/SSI, enable SPI2
void SPI_Motor_Config(void) {
	//Make our stm32 master
	SPI2->CR1 |= (1 << 2);
	// Enable Software Slave Management and Internal Slave Select (SSM=1, SSI=1)
	SPI2->CR1 |= (1 << 9) | (1 << 8);
	//Baud rate 1/8
	SPI2->CR1 &= ~(7 << 3);
	SPI2->CR1 |=  (2 << 3);
	//Peripheral clock enable
	SPI2->CR1 |= (1 << 6);
}

void SPI_Pin_Config(void) {
	// PB13, PB14, PB15 pins -> Alternate Function mode
	GPIOB->MODER &= ~((3 << 26) | (3 << 28) | (3 << 30));
	GPIOB->MODER |=  ((2 << 26) | (2 << 28) | (2 << 30));
	//Connect the MOSI,MISO,SCLK pins to SPI unit
	GPIOB->AFR[1] &= ~((15 << 20) | (15 << 24) | (15 << 28));
	GPIOB->AFR[1] |=  ((5  << 20) | (5  << 24) | (5  << 28));
	// Set the upper pins to high speed mode
	GPIOB->OSPEEDR |= ((3 << 26) | (3 << 28) | (3 << 30));
	// PC8 (CSN) pin -> output mode:
	GPIOC->MODER &= ~(3 << 16);
	GPIOC->MODER |=  (1 << 16);
	//Arrange CSN pin to push-pull mode
	GPIOC->OTYPER &= ~(1 << 8);
	// PB2 (CE) pin -> output mode:
	GPIOB->MODER &= ~(3 << 4);
	GPIOB->MODER |=  (1 << 4);
	// Arrange CE to Push Pull mode
	GPIOB->OTYPER &= ~(1 << 2);
	// Safe initial pin states
	CSN_HIGH();
	CE_LOW();
}
