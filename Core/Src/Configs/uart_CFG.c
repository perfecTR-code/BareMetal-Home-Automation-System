#include "uart_CFG.h"

void initClocks(void) {
	//Enable A Clock
    RCC->AHB1ENR |= (1u << 0);
    //Enable USART 1 Clock
    RCC->APB2ENR |= (1u << 4);
}

void configUart1Pins(void) {
    // "u" means unsigned.(MISRA C rules)
	// Connect the corresponding pins to exact communication unit.
    GPIOA->MODER &= ~((3u << (9 * 2)) | (3u << (10 * 2)));
    GPIOA->MODER |=  ((2u << (9 * 2)) | (2u << (10 * 2)));

    // I have boosted those pins to make myself sure about the pin speed problem.I want the pins to not be late
    GPIOA->OSPEEDR |= ((3u << (9 * 2)) | (3u << (10 * 2)));

    // In UART protocol pins must generally be in true (3V) state. So I enabled the pull-up mode.
    // Besides that provides pins to don't get affected by the noise. Noise makes pins 1 and 0 randomly which is unexpected.
    GPIOA->PUPDR &= ~((3u << (9 * 2)) | (3u << (10 * 2)));
    GPIOA->PUPDR |=  ((1u << (9 * 2)) | (1u << (10 * 2)));

    // Map PA9 and PA10 to Alternate Function 7 (AF7) which connects them to the USART1 peripheral engine
    GPIOA->AFR[1] &= ~((0xFu << 4) | (0xFu << 8));
    GPIOA->AFR[1] |=  ((7u << 4)   | (7u << 8));
}

void configUart1(void) {
	//Reset those before we make process
    USART1->CR1 = 0;
    USART1->CR2 = 0;
    USART1->CR3 = 0;

    // The line that makes baud = 9600 because HC-06 Module needs it.
    USART1->BRR = (104 << 4) | (3 << 0);

    // Enable UE (USART Enable) bit (Bit 13)
    USART1->CR1 |= (1u << 13);

    // Enable RE (Receiver Enable) bit (Bit 3)
    USART1->CR1 |= (1u << 3);

    // Enable TE (Transmitter Enable) bit (Bit 2)
    USART1->CR1 |= (1u << 2);
}
