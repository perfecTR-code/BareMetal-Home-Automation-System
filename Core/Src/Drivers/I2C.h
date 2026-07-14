#ifndef I2C_H_
#define I2C_H_

#include "../Configs/I2C_CFG.h"
#include <stdbool.h>

void System_Init(void);
void Delay_ms(uint32_t ms);

void I2C1_Start(void);
void I2C1_WriteAddress(uint8_t address);
void I2C1_WriteData(uint8_t data);
void I2C1_Stop(void);

uint8_t Data_Divider_High(uint8_t data);
uint8_t Data_Divider_Low(uint8_t data);

uint8_t Prepare_LCD_Packet(uint8_t nibble, uint8_t Is_En_True, uint8_t rs_mode);

void LCD_Send_High_2_Nibble(uint8_t high_data);
void LCD_Send_Low_2_Nibble(uint8_t low_data);

void LCD_Send_Command(uint8_t cmd);
void LCD_Init(void);

void General(uint8_t address, uint8_t data);
void I2C_Main_Function(char* arr, int size);

#endif /* I2C_H_ */
