#include "stm32f4xx.h"
#include <string.h>
#include <stdbool.h>
#include "Drivers/uart.h"
#include "Drivers/I2C.h"
#include "Drivers/SPI.h"
#include "nRF24.h"
#include "Configs/TIM-Config.h"

int main(void) {
    //Activate all hardware modules
    initUART();
    System_Init();   // I2C
    LCD_Init();      // LCD driver
    Pin_Configs();
    TIM1_Settings();

    // 2. SPI and nRF24 initialization
    SPI_Clock_Init();
    SPI_Pin_Config();
    SPI_Motor_Config();
    nRF24_Init();

    // Set the nRF24 adress
    uint8_t address[5] = {'0', '0', '0', '0', '1'};
    nRF24_Set_TX_Address(address);
    nRF24_Set_RX_Address(address);

    Servo_Motor Servo;

    transmitStrUart("STM32 system is ready. Command is expected.\r\n");

    char welcome_msg[] = "System Ready...";
    I2C_Main_Function(welcome_msg, strlen(welcome_msg));

    char data[100] = {0};

    while(1) {
        memset(data, 0, sizeof(data));
        Uart_Receiver(data, 100); // Wait until new commands comes here

        transmitStrUart("Command Taken: ");
        transmitStrUart(data);
        transmitStrUart("\r\n");

        LCD_Send_Command(0x01); //Reset the Cursor
        Delay_ms(2);

        I2C_Main_Function(data, strlen(data));

        //Open State
        if (strcmp(data, "AC") == 0) {

            Servo_Motor_Setter(&Servo, 90, 3);
            Execute_Servo_Motor(&Servo);

            // Send command to Ardunio MCU Unit
            nRF24_Send_Packet((uint8_t*)"AC", 2);
            transmitStrUart("Wireless command 'AC' sent!\r\n");
        }
        else if (strcmp(data, "KAPAT") == 0) {
            Servo_Motor_Setter(&Servo, 0, 2);  //Close the servo
            Execute_Servo_Motor(&Servo);

            // Send command to Ardunio MCU Unit
            nRF24_Send_Packet((uint8_t*)"KAPAT", 5);
            transmitStrUart("Wireless command 'KAPAT' sent!\r\n");
        }
    }
}
