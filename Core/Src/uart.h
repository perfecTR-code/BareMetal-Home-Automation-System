#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h" // uint8_t gibi tipleri tanıması için gerekebilir

// Fonksiyonlarının prototiplerini buraya ekledik:
void initClocks(void);
void configUart1Pins(void);
void configUart1(void);
void initUART(void);
void transmitUart(const uint8_t data);
void transmitStrUart(const char* str);
void Uart_Receiver(char* arr, int size);

#endif /* UART_H_ */
