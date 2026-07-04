#include "stm32f4xx.h"
#include <stdbool.h>
#include <string.h>
#include "I2C.h"
#include "TIM-Config.h" // Doğru ve güncel .h dosyası eklendi, eskiler silindi

bool Is_Idle = 1;

void Servo_Motor_Setter(Servo_Motor *Servo, uint32_t Angle, uint32_t Holding_Time){
	if(Angle < 181 && Angle >= 0){
		Servo->Angle = Angle;
	}
	if(Holding_Time < 20 && Holding_Time > 0 ){
		Servo->Holding_Time = Holding_Time;
	}
	else{
		LCD_Send_Command(0x01);
		char arr[] = "Invalid Angle or Holding Time";
		I2C_Main_Function(arr, strlen(arr));
		return;
	} // .
}

uint32_t Convert_Angle_To_CCR(uint32_t angle) {
    if (angle > 180) {
        angle = 180;
    }
    return (angle * 1000 / 180) + 1000;
}

void Execute_Servo_Motor(Servo_Motor *Servo){
	TIM1->CCR1 = (uint16_t)Convert_Angle_To_CCR(Servo->Angle);
	Delay_ms(Servo->Holding_Time * 1000);
	TIM1->CCR1 = 1000;
}

void TIM1_Settings(void){
	RCC->APB2ENR |= (1<<0);
	TIM1->PSC = 15;
	TIM1->ARR = 19999;

	TIM1->CCMR1 &= ~(3 << 0);
	TIM1->CCMR1 &= ~(0xF << 3);
	TIM1->CCMR1 |= (6 << 4);
	TIM1->CCMR1 |= (1 << 3);

	TIM1->CCER &= ~(1 << 0);
	TIM1->CCER |= (1 << 0);

	TIM1->BDTR |= (1 << 15);
	TIM1->CR1 |= (1 << 0);
	TIM1->CCR1 = 1000;
}

void Pin_Configs(void){
	RCC->AHB1ENR |= (1 << 0);

	GPIOA->MODER &= ~(3 << 16);
	GPIOA->MODER |=  (2 << 16);

	GPIOA->AFR[1] &= ~(15 << 0);
	GPIOA->AFR[1] |=  (1 << 0);

	GPIOA->OSPEEDR &= ~(3 << 16);
	GPIOA->OSPEEDR |=  (3 << 16);
}
