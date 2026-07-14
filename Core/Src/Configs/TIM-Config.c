#include "stm32f4xx.h"
#include <stdbool.h>
#include <string.h>
#include "../Drivers/I2C.h"
#include "TIM-Config.h" // Doğru ve güncel .h dosyası eklendi, eskiler silindi

// Sistemin meşguliyet durumunu tutan flag (1: Boşta, 0: Meşgul)
bool Is_Idle = 1;

void Servo_Motor_Setter(Servo_Motor *Servo, uint32_t Angle, uint32_t Holding_Time){
	//Chech if angel is between 0 and 180 degree
	if(Angle < 181 && Angle >= 0){
		Servo->Angle = Angle;
	}
	// Check the waiting time
	if(Holding_Time < 20 && Holding_Time > 0 ){
		Servo->Holding_Time = Holding_Time;
	}
	else{
		// In invalid input case, throw eroor message to LED panel
		LCD_Send_Command(0x01); // LCD Clear Command
		char arr[] = "Invalid Angle or Holding Time";
		I2C_Main_Function(arr, strlen(arr));
		return;
	}
}


//0 degree -> 1000 us (1 ms) | 180 degree -> 2000 us (2 ms)

uint32_t Convert_Angle_To_CCR(uint32_t angle) {
    if (angle > 180) {
        angle = 180;
    }
    return (angle * 1000 / 180) + 1000;
}


void Execute_Servo_Motor(Servo_Motor *Servo){
	// Execute
	TIM1->CCR1 = (uint16_t)Convert_Angle_To_CCR(Servo->Angle);

	// Wait
	Delay_ms(Servo->Holding_Time * 1000);

	// Go to start position when process ends
	TIM1->CCR1 = 1000;
}


void TIM1_Settings(void){
	//Enable TIM1 clock gating)
	RCC->APB2ENR |= (1 << 0);

	//Configure Timer Frequency (16MHz / (15 + 1) = 1MHz -> 1 us counting period)
	TIM1->PSC = 15;

	//Set PWM Period ->20ms
	TIM1->ARR = 19999;

	//Configure Channel 1 as Output
	TIM1->CCMR1 &= ~(3 << 0);


	TIM1->CCMR1 &= ~(0xF << 3); // Clear
	// PWM mode = if CNT<CCMR1 then send signal
	TIM1->CCMR1 |= (6 << 4);
	//Preload mode activation that makes newly coming value gets stored
	TIM1->CCMR1 |= (1 << 3);

	// 6. Enable Capture/Compare 1 Output: CC1E = 1 (CCER register, bit 0)
	TIM1->CCER &= ~(1 << 0);
	TIM1->CCER |= (1 << 0);

	// 7. Enable Main Output for Advanced Control Timer (TIM1): MOE = 1 (BDTR register, bit 15)
	TIM1->BDTR |= (1 << 15);

	// 8. Start the Timer: CEN = 1 (CR1 register, bit 0)
	TIM1->CR1 |= (1 << 0);

	// Set initial pulse width to 1 ms (corresponds to 0 degrees position)
	TIM1->CCR1 = 1000;
}


void Pin_Configs(void){
	// 1. Enable GPIOA Port Peripheral Clock (AHB1 peripheral clock enable register, bit 0)
	RCC->AHB1ENR |= (1 << 0);

	// 2. Configure PA8 Pin to Alternate Function Mode: MODER8[1:0] = 10 (value 2)
	GPIOA->MODER &= ~(3 << 16);
	GPIOA->MODER |=  (2 << 16);

	// 3. Map PA8 Pin to TIM1_CH1 (Alternate Function High Register - AFR[1] / AFRH)
	// PA8 corresponds to the first 4 bits (0,1,2,3) of the AFRH register. Select AF1 (0001) for TIM1.
	GPIOA->AFR[1] &= ~(15 << 0);
	GPIOA->AFR[1] |=  (1 << 0);

	// 4. Set PA8 Output Speed to Very High to make sure no problem will be created because of the time
	GPIOA->OSPEEDR &= ~(3 << 16);
	GPIOA->OSPEEDR |=  (3 << 16);
}
