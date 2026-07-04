#include "stm32f4xx.h"

// This is here to debug the protocol
volatile uint32_t loop_counter = 0;

void initClocks(void);
void configUart1Pins(void);
void configUart1(void);
void initUART(void);
void transmitUart(const uint8_t data);
void transmitStrUart(const char* str);
void Uart_Receiver(char* arr, int size);

//This function is here to receive the data char by char
void Uart_Receiver(char* arr, int size) {
    int i = 0;
    uint8_t coming_char = 0;

    while (i < (size - 1)) {
        // Wait RXNE flag (Read Data Register Not Empty)
        while (!(USART1->SR & (1 << 5)));
        //Copy data and paste it to coming_char
        coming_char = (uint8_t)(USART1->DR & 0xFF);

        // If coming the character is \r do not add it into the array
        if (coming_char == '\r') {
            continue;
        }

        // If the coming char is \n that means it is the end of data so do not add it into array and break the loop
        if (coming_char == '\n') {
            break;
        }

        // Place the char into array and increase the "i"
        arr[i] = coming_char;
        i++;
    }

    // Append the end char
    arr[i] = '\0';
}

void initClocks(void) {
	//Enable A Clock
    RCC->AHB1ENR |= (1u << 0);
    //Enable USART 1 Clock
    RCC->APB2ENR |= (1u << 4);
}

void configUart1Pins(void) {
    // "u" means unsigned.(MISRA C rules)
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

void initUART(void) {
    initClocks();
    configUart1Pins();
    configUart1();
}

void transmitUart(const uint8_t data) {
	// Wait until the TXE flag is set that is meaning DR is ready for new data
    while (!(USART1->SR & (1u << 7)));
    // Paste the data to register.
    USART1->DR = data;
}

void transmitStrUart(const char* str) {
    // NULL pointer protection
    if (str == NULL) {
        return;
    }
    // "\0" means end of data so maintain process till you see "\0"
    while (*str != '\0') {
        transmitUart(*str);
        // Advance the pointer to the next memory address
        str++;
    }
}

