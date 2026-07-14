#include "smart_blind.h"
#include "../Drivers/I2C.h"
#include "../Configs/TIM-Config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Modüler Panjur Komut İşleyici (Servo Pozisyonunu Sabit Tutar)
 */
void Handle_Smart_Blind_Command(char* data, Servo_Motor* Servo) {
    if (data == NULL || Servo == NULL) return;

    char lcd_buf[17] = {0};

    LCD_Send_Command(0x01); // Ekranı temizle
    Delay_ms(2);

    if (strcmp(data, "AC") == 0) {
        char msg[] = "TAM ACILDI %100";
        I2C_Main_Function(msg, strlen(msg));
        Servo->Angle = 180;
        TIM1->CCR1 = (uint16_t)Convert_Angle_To_CCR(Servo->Angle);
    }
    else if (strcmp(data, "KAPAT") == 0) {
        char msg[] = "KAPATILDI %0";
        I2C_Main_Function(msg, strlen(msg));
        Servo->Angle = 0;
        TIM1->CCR1 = (uint16_t)Convert_Angle_To_CCR(Servo->Angle);
    }
    else if (data[0] == 'P' || data[0] == 'p') {
        int percentage = atoi(&data[1]);
        if (percentage < 0) percentage = 0;
        if (percentage > 100) percentage = 100;

        uint32_t target_angle = (uint32_t)((percentage * 180) / 100);

        snprintf(lcd_buf, sizeof(lcd_buf), "Acs:%d%% Ac:%d", percentage, (int)target_angle);
        I2C_Main_Function(lcd_buf, strlen(lcd_buf));

        Servo->Angle = target_angle;
        TIM1->CCR1 = (uint16_t)Convert_Angle_To_CCR(Servo->Angle);
    }
    else {
        I2C_Main_Function(data, strlen(data));
    }
}
