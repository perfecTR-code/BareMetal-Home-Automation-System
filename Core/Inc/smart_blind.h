#ifndef SMART_BLIND_H_
#define SMART_BLIND_H_

#include "stm32f4xx.h"
#include "TIM-Config.h"
#include "I2C.h"

// Modüler Panjur Komut İşleyici Fonksiyonu Prototipleri
void Handle_Smart_Blind_Command(char* data, Servo_Motor* Servo);

#endif /* SMART_BLIND_H_ */
