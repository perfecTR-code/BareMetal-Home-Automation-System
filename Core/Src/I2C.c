#include "i2c.h"

// Scheduler
void Delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < (ms * 1600); i++) {
        __NOP(); // Make Cpu Busy
    }
}

// Data Division
uint8_t Data_Divider_High(uint8_t data){
    return data & 0xF0; //Makes bit xxxx 0000 type
}

uint8_t Data_Divider_Low(uint8_t data){
    return data & 0x0F; //Makes bit 0000 xxxx type
}

// LCD Pack Preparator
uint8_t Prepare_LCD_Packet(uint8_t Nibble, uint8_t Is_En_True, uint8_t Rs_Mode) {
    uint8_t pack = Nibble | 0x08 | Rs_Mode;
    if(Is_En_True) {
        pack |= (1 << 2);
    }
    return pack;
}

// All LCD Operations
void LCD_Send_High_2_Nibble(uint8_t high_data) {
    I2C1_WriteData(Prepare_LCD_Packet(high_data, true, 1));
    I2C1_WriteData(Prepare_LCD_Packet(high_data, false, 1));
}

void LCD_Send_Low_2_Nibble(uint8_t low_data) {
    uint8_t Sent_Data = (low_data << 4) & 0xF0;
    I2C1_WriteData(Prepare_LCD_Packet(Sent_Data, true, 1));
    I2C1_WriteData(Prepare_LCD_Packet(Sent_Data, false, 1));
}

void LCD_Send_Command(uint8_t cmd) {
    uint8_t High_Nibble = cmd & 0xF0;
    uint8_t Low_Nibble = (cmd << 4) & 0xF0;

    I2C1_Start();
    I2C1_WriteAddress(0x27);

    I2C1_WriteData(Prepare_LCD_Packet(High_Nibble, true, 0));
    I2C1_WriteData(Prepare_LCD_Packet(High_Nibble, false, 0));
    I2C1_WriteData(Prepare_LCD_Packet(Low_Nibble, true, 0));
    I2C1_WriteData(Prepare_LCD_Packet(Low_Nibble, false, 0));

    I2C1_Stop();
    Delay_ms(2);
}

void LCD_Init(void) {
    Delay_ms(50);

    I2C1_Start(); I2C1_WriteAddress(0x27);
    I2C1_WriteData(0x30 | 0x08 | (1 << 2)); I2C1_WriteData(0x30 | 0x08);
    I2C1_Stop(); Delay_ms(5);

    I2C1_Start(); I2C1_WriteAddress(0x27);
    I2C1_WriteData(0x30 | 0x08 | (1 << 2)); I2C1_WriteData(0x30 | 0x08);
    I2C1_Stop(); Delay_ms(1);

    I2C1_Start(); I2C1_WriteAddress(0x27);
    I2C1_WriteData(0x30 | 0x08 | (1 << 2)); I2C1_WriteData(0x30 | 0x08);
    I2C1_Stop(); Delay_ms(1);

    I2C1_Start(); I2C1_WriteAddress(0x27);
    I2C1_WriteData(0x20 | 0x08 | (1 << 2)); I2C1_WriteData(0x20 | 0x08);
    I2C1_Stop(); Delay_ms(5);

    LCD_Send_Command(0x28);
    LCD_Send_Command(0x0C);
    LCD_Send_Command(0x01);
    Delay_ms(2);
    LCD_Send_Command(0x06);
}

// Basic I2C1 Operations
void I2C1_Start(void){
    I2C1->CR1 |= (1 << 8);
    while(!(I2C1->SR1 & (1 << 0)));
}

void I2C1_WriteAddress(uint8_t address){
    if(address <= 127){
        I2C1->DR = (address << 1);
        while(!(I2C1->SR1 & (1 << 1)));

        uint8_t dummy1 = I2C1->SR1; uint8_t dummy2 = I2C1->SR2;
        (void)dummy1; (void)dummy2;
    }
}

void I2C1_WriteData(uint8_t data){
    I2C1->DR = data;
    while(!(I2C1->SR1 & (1 << 2)));
}

void I2C1_Stop(void){
    I2C1->CR1 |= (1 << 9);
}

// Manager Functions
void General(uint8_t address, uint8_t data){
    I2C1_Start();
    I2C1_WriteAddress(address);

    LCD_Send_High_2_Nibble(Data_Divider_High(data));
    LCD_Send_Low_2_Nibble(Data_Divider_Low(data));

    I2C1_Stop();
}

void I2C_Main_Function(char* arr, int size){
    for(int i = 0; i < size; i++){
        uint8_t harf = (uint8_t)(*(arr + i));
        General(0x27, harf);
    }
}

// System Initiation
void System_Init(void) {
    RCC->AHB1ENR |= (1 << 1);
    RCC->APB1ENR |= (1 << 21);

    I2C1->CR2 &= ~(63 << 0);
    I2C1->CR2 |= (16 << 0);

    I2C1->CCR &= ~(2047 << 0);
    I2C1->CCR |= (80 << 0);

    I2C1->TRISE &= ~(63 << 0);
    I2C1->TRISE |= (17 << 0);

    GPIOB->MODER &= ~(15 << 16);
    GPIOB->MODER |= (1 << 17) | (1 << 19);

    GPIOB->OTYPER |= (1 << 8) | (1 << 9);

    GPIOB->OSPEEDR &= ~(15 << 16);
    GPIOB->OSPEEDR |= (1 << 17) | (1 << 19);

    GPIOB->AFR[1] &= ~((15 << 0) | (15 << 4));
    GPIOB->AFR[1] |= (4 << 0) | (4 << 4);

    I2C1->CR1 |= (1 << 0);
}
