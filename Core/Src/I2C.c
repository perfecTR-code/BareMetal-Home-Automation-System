#include "stm32f4xx.h"
#include <stdbool.h>

// ====================================================================
                            //Function Prototypes
// ====================================================================
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

// Scheduler
void Delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < (ms * 1600); i++) {
        __NOP(); // Make Cpu Busy
    }
}

// ********************************************************************
                    // Data Division
// ********************************************************************
uint8_t Data_Divider_High(uint8_t data){
    return data & 0xF0; //Makes bit xxxx 0000 type
}

uint8_t Data_Divider_Low(uint8_t data){
    return data & 0x0F; //Makes bit 0000 xxxx type
}

// ********************************************************************
                                //  Packet
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
                        // All LCD Operations
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
    /* ====================================================================
       STEP 1: PERIPHERAL CLOCK ACTIVATION (RCC LAYER)
       ==================================================================== */
    // Enable the clock gating for GPIOB peripheral. Without this, modifying GPIOB registers has no effect.
    RCC->AHB1ENR |= (1 << 1);

    // Enable the clock gating for the I2C1 hardware engine located on the APB1 bus.
    RCC->APB1ENR |= (1 << 21);

    /* ====================================================================
       STEP 2: PERIPHERAL FREQUENCY CONFIGURATION (I2C_CR2 REGISTER)
       ==================================================================== */
    // Clear the FREQ[5:0] bits (lower 6 bits) to safely overwrite the frequency configuration.
    I2C1->CR2 &= ~(63 << 0);

    // Set the internal peripheral clock frequency to 16MHz (Matches the APB1 bus speed).
    // This value is critical for the internal hardware timers to accurately calculate I2C timings.
    I2C1->CR2 |= (16 << 0);

    /* ====================================================================
       STEP 3: CLOCK CONTROL CONFIGURATION (I2C_CCR REGISTER)
       ==================================================================== */
    // Clear the 11-bit CCR[10:0] mask to reset the clock control register field.
    I2C1->CCR &= ~(2047 << 0);

    // Configure the Clock Control Register for Standard Mode (100kHz).
    // Formula: CCR = T_high / T_pclk1 -> For 100kHz (10us period), T_high = 5us. 5us / (1 / 16MHz) = 80.
    I2C1->CCR |= (80 << 0);

    /* ====================================================================
       STEP 4: RISE TIME CONFIGURATION (I2C_TRISE REGISTER)
       ==================================================================== */
    // Clear the 6-bit TRISE[5:0] mask to safely write the maximum allowed rise time.
    I2C1->TRISE &= ~(63 << 0);
    
    // Set the maximum allowed t_rise for Standard Mode (Max = 1000ns / 1us).
    // Formula: TRISE = (Max_Rise_Time / T_pclk1) + 1 -> (1000ns / 62.5ns) + 1 = 16 + 1 = 17.
    I2C1->TRISE |= (17 << 0);

    /* ====================================================================
       STEP 5: GPIO PORT B MULTIPLEXING AND CONFIGURATION (PB8 & PB9)
       ==================================================================== */
    // Clear the Mode bits for Pin 8 and Pin 9 (Bits 19:16) to prepare for re-configuration.
    GPIOB->MODER &= ~(15 << 16);

    // Set PB8 and PB9 to Alternate Function Mode (0101 configuration patterns -> Bit 17 and Bit 19).
    GPIOB->MODER |= (1 << 17) | (1 << 19);

    // Force PB8 (SCL) and PB9 (SDA) into Open-Drain output configuration.
    // I2C protocol requires Open-Drain lines combined with external pull-up resistors to prevent bus contention.
    GPIOB->OTYPER |= (1 << 8) | (1 << 9);

    // Reset the OSPEEDR speed configuration mask for Pin 8 and Pin 9 (Bits 19:16).
    GPIOB->OSPEEDR &= ~(15 << 16);

    // Drive PB8 and PB9 with High-Speed signal output configurations to ensure crisp edge transitions.
    GPIOB->OSPEEDR |= (1 << 17) | (1 << 19);

    // Clear the Alternate Function High Register (AFR[1]) fields for pins 8 and 9 (Bits 7:0).
    GPIOB->AFR[1] &= ~((15 << 0) | (15 << 4));

    // Hardwire PB8 and PB9 pathways to Alternate Function 4 (AF4) which is designated for the I2C1 engine.
    GPIOB->AFR[1] |= (4 << 0) | (4 << 4);

    /* ====================================================================
       STEP 6: PERIPHERAL IGNITION (I2C_CR1 REGISTER)
       ==================================================================== */
    // Fire peripheral power configurations by setting the PE (Peripheral Enable) bit high.
    // The I2C1 hardware engine is now alive and actively driving the physical bus lanes.
    I2C1->CR1 |= (1 << 0);
}

