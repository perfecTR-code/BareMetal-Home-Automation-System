#include "SPI.h"

void Delay_us(uint32_t us) {
    SysTick->LOAD = (us * 16) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = (1 << 2) | (1 << 0);
    while (!(SysTick->CTRL & (1 << 16)));
    SysTick->CTRL = 0;
}

void SPI_Send_Data(uint8_t data) {
    // 1. Wait until transmitter buffer is empty (TXE = 1)
    while (!(SPI2->SR & (1 << 1)));
    
    // 2. Write data to data register
    SPI2->DR = data;
    
    // 3. Wait until TXE is set again
    while (!(SPI2->SR & (1 << 1)));
    
    // 4. Wait until SPI is not busy (BSY = 0) to ensure transmission is physically done
    while (SPI2->SR & (1 << 7));
    
    // 5. Clear RXNE (Receive buffer not empty) flag by reading DR (prevents overrun)
    uint8_t dummy = SPI2->DR;
    (void)dummy;
}

uint8_t SPI_Receive_Data(void) {
    // 1. Wait until transmitter buffer is empty (TXE = 1)
    while (!(SPI2->SR & (1 << 1)));
    
    // 2. Send dummy byte (0xFF) to generate SPI clocks for reception
    SPI2->DR = 0xFF;
    
    // 3. Wait until TXE is set
    while (!(SPI2->SR & (1 << 1)));
    
    // 4. Wait until SPI is not busy (BSY = 0)
    while (SPI2->SR & (1 << 7));
    
    // 5. Wait until RXNE is set (data received)
    while (!(SPI2->SR & (1 << 0)));
    
    // 6. Return the received data from DR
    return (uint8_t)SPI2->DR;
}

uint8_t SPI_Tranceive(uint8_t data) {
    // 1. Wait until TXE (Transmit buffer empty) flag is set
    while (!(SPI2->SR & (1 << 1)));
    
    // 2. Write data to DR register (initiates SPI clock and transmission)
    SPI2->DR = data;
    
    // 3. Wait until RXNE (Receive buffer not empty) flag is set
    while (!(SPI2->SR & (1 << 0)));
    
    // 4. Read received byte from DR register (also automatically clears RXNE flag)
    return (uint8_t)SPI2->DR;
}

