#include "stm32f4xx.h"
#include <string.h>
#include <stdbool.h>
#include "uart.h"
#include "i2c.h"
#include "TIM-Config.h" // Servo ve Zamanlayıcı kütüphanemiz eklendi

int main(void) {
    // 1. Tüm Donanım Modüllerini İlklendir
    initUART();
    System_Init();   // I2C donanımı
    LCD_Init();      // LCD Ekran sürücüsü
    Pin_Configs();   // Servo motor pini (D7 / PA8)
    TIM1_Settings(); // Servo motor PWM Zamanlayıcısı

    // 2. Servo Motor Nesnesini Oluştur
    Servo_Motor panjur_servosu;

    transmitStrUart("STM32 system is ready. Command is expected.\r\n");

    char welcome_msg[] = "System Ready...";
    I2C_Main_Function(welcome_msg, strlen(welcome_msg));

    char data[100] = {0};

    while(1) {
        memset(data, 0, sizeof(data));
        Uart_Receiver(data, 100); // Yeni komut gelene kadar burada bloke olur

        transmitStrUart("Command Taken: ");
        transmitStrUart(data);
        transmitStrUart("\r\n");

        LCD_Send_Command(0x01);
        Delay_ms(2);

        I2C_Main_Function(data, strlen(data));

        // 3. UART Komutuna Göre Servoyu Tetikle
        if (strcmp(data, "AC") == 0) {
            Servo_Motor_Setter(&panjur_servosu, 90, 3); // 90 derece, 3 saniye basılı tut
            Execute_Servo_Motor(&panjur_servosu);
        }
        else if (strcmp(data, "KAPAT") == 0) {
            Servo_Motor_Setter(&panjur_servosu, 0, 2);  // Başlangıç noktasına çek, 2 saniye tut
            Execute_Servo_Motor(&panjur_servosu);
        }
    }
}
