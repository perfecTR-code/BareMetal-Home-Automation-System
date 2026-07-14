#ifndef TIMCONFIG_H_
#define TIMCONFIG_H_

#include "stm32f4xx.h"
#include <stdbool.h>

// Servo motor nesne şablonu
typedef struct {
    uint32_t Angle;
    uint32_t Holding_Time;
} Servo_Motor;

// Fonksiyon Prototipleri
void TIM1_Settings(void);
void Pin_Configs(void);
void Delay_ms(uint32_t ms);

// Servo Yardımcı Fonksiyonları
void Servo_Motor_Setter(Servo_Motor *Servo, uint32_t Angle, uint32_t Holding_Time);
uint32_t Convert_Angle_To_CCR(uint32_t angle);
void Execute_Servo_Motor(Servo_Motor *Servo);

#endif /* TIMCONFIG_H_ */
