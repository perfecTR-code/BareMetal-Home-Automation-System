#ifndef SRC_SPI_CFG_H_
#define SRC_SPI_CFG_H_

#include "stm32f4xx.h"

/*
 *********************************************************************
                             PINOUT
    CSN  -> PC_8
    MOSI -> PB_15
    MISO -> PB14
    PB_13 -> SCLK
    PB_2 -> CE
 *********************************************************************
*/

#define CSN_LOW()   (GPIOC->BSRR = (1 << 24))
#define CSN_HIGH()  (GPIOC->BSRR = (1 << 8))
#define CE_LOW()    (GPIOB->BSRR = (1 << 18))
#define CE_HIGH()   (GPIOB->BSRR = (1 << 2))

void SPI_Clock_Init(void);
void SPI_Pin_Config(void);
void SPI_Motor_Config(void);

#endif /* SRC_SPI_CFG_H_ */
