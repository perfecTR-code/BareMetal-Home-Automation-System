#include "uart.h"
#include <string.h>

// This is here to debug the protocol
volatile uint32_t loop_counter = 0;

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
