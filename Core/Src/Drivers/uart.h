#ifndef UART_H_
#define UART_H_

#include "../Configs/uart_CFG.h"

void initUART(void);
void transmitUart(const uint8_t data);
void transmitStrUart(const char* str);
void Uart_Receiver(char* arr, int size);

#endif /* UART_H_ */
