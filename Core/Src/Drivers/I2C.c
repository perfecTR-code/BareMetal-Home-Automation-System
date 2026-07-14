#include "I2C.h"

// Scheduler
void Delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < (ms * 1600); i++) {
        __NOP(); // Make Cpu Busy
    }
}

// ********************************************************************
//                     Data Division
// ********************************************************************
uint8_t Data_Divider_High(uint8_t data){
    return data & 0xF0; //Makes bit xxxx 0000 type
}

uint8_t Data_Divider_Low(uint8_t data){
    return data & 0x0F; //Makes bit 0000 xxxx type
}

// ********************************************************************
//                                 Packet
// ********************************************************************
// LCD Pack Preparator
uint8_t Prepare_LCD_Packet(uint8_t Nibble, uint8_t Is_En_True, uint8_t Rs_Mode) {
    //Prepare bit to that form,Nible = N, (8 bit form ----> N N N N Light EN RW Rs)
    uint8_t pack = Nibble | 0x08 | Rs_Mode;

    //Enable Bit setter part
    if(Is_En_True) {
        pack |= (1 << 2); // İf EN=1 then add it into corresponding bit
    }

    return pack;
}

// ********************************************************************
//                         All LCD Operations
// ********************************************************************
void LCD_Send_High_2_Nibble(uint8_t high_data) {
    // Necessary values is setted below here.
    I2C1_WriteData(Prepare_LCD_Packet(high_data, true, 1));
    I2C1_WriteData(Prepare_LCD_Packet(high_data, false, 1));
}

void LCD_Send_Low_2_Nibble(uint8_t low_data) {
    // bit form preparation --> N N N N Light EN RW Rs
    uint8_t Sent_Data = (low_data << 4) & 0xF0;

    // First make EN mode 1 then do EN mode 0 and send...
    I2C1_WriteData(Prepare_LCD_Packet(Sent_Data, true, 1));  // EN = 1
    I2C1_WriteData(Prepare_LCD_Packet(Sent_Data, false, 1)); // EN = 0
}

                        // Main LCD data sender Function
void LCD_Send_Command(uint8_t cmd) {
    uint8_t High_Nibble = cmd & 0xF0;
    uint8_t Low_Nibble = (cmd << 4) & 0xF0;
    //It starts beginning operations
    I2C1_Start();
    //Sends Corresponding address
    I2C1_WriteAddress(0x27);

    // In the first place we send high nibble by making EN 0 from 1
    //Rs mode = 0
    I2C1_WriteData(Prepare_LCD_Packet(High_Nibble, true, 0));  //  EN = 1
    I2C1_WriteData(Prepare_LCD_Packet(High_Nibble, false, 0)); // EN = 0
    //Send low nibbles with the same technique
    I2C1_WriteData(Prepare_LCD_Packet(Low_Nibble, true, 0));  // EN = 1
    I2C1_WriteData(Prepare_LCD_Packet(Low_Nibble, false, 0)); // EN = 0

    I2C1_Stop();
    // Time for recovering
    Delay_ms(2);
}

// Solution is Copied from internet and it got integrated
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

// ********************************************************************
//                          Basic I2C1 Operations
// ********************************************************************
void I2C1_Start(void){
    I2C1->CR1 |= (1 << 8); // Trigger Start bit
    while(!(I2C1->SR1 & (1 << 0))); // Wait hardware response for start condition
}

void I2C1_WriteAddress(uint8_t address){
    if(address <= 127){
        I2C1->DR = (address << 1); // Slide address to 1 unit left because rightest bit must be 0 that means write mode on.
        while(!(I2C1->SR1 & (1 << 1))); // Wait other hardware response for data safety

        // This is here to prevent compiler delete our precious code
        uint8_t dummy1 = I2C1->SR1; uint8_t dummy2 = I2C1->SR2;
        (void)dummy1; (void)dummy2;
    }
}

void I2C1_WriteData(uint8_t data){
    I2C1->DR = data; // Paste data to Data Register
    while(!(I2C1->SR1 & (1 << 2))); // Wait other hardware's Acknowledge bit
}

void I2C1_Stop(void){
    I2C1->CR1 |= (1 << 9); // Trigger Stop bit that means data sending is over.
}

// ********************************************************************
//                          Manager Functions
// ********************************************************************
void General(uint8_t address, uint8_t data){
    //Starts Communication
    I2C1_Start();
    //Writes Corresponding address (0x27)
    I2C1_WriteAddress(address);

    //Divides the data and prepares it for the right form then sends it || Data --> Right Nibble Form
    LCD_Send_High_2_Nibble(Data_Divider_High(data));
    LCD_Send_Low_2_Nibble(Data_Divider_Low(data));

    //Communication is over
    I2C1_Stop();
}

//Main Function
void I2C_Main_Function(char* arr, int size){
    for(int i = 0; i < size; i++){
        uint8_t harf = (uint8_t)(*(arr + i));
        General(0x27, harf); // Harfleri sırayla o canavar Burst çarkına sokuyoruz
    }
}

// ********************************************************************
//                      System Initiation
// ********************************************************************
void System_Init(void) {
    I2C_Clock_Init();
    I2C_Pin_Config();
    I2C_Peripheral_Config();
}
