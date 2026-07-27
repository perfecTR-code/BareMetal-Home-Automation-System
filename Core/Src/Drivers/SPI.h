#ifndef SRC_SPI_H_
#define SRC_SPI_H_

#include "../Configs/SPI_CFG.h"

void Delay_us(uint32_t us);
void SPI_Send_Data(uint8_t data);
uint8_t SPI_Receive_Data(void);
uint8_t SPI_Tranceive(uint8_t data);

#endif /* SRC_SPI_H_ */
