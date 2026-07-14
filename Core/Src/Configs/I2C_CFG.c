#include "I2C_CFG.h"

// Enable peripheral clocks for GPIOB and I2C1
void I2C_Clock_Init(void) {
    // Enable the clock gating for GPIOB peripheral. Without this, modifying GPIOB registers has no effect.
    RCC->AHB1ENR |= (1 << 1);

    // Enable the clock gating for the I2C1 hardware engine located on the APB1 bus.
    RCC->APB1ENR |= (1 << 21);
}

// Multiplex PB8 (SCL) and PB9 (SDA) pins to I2C1 AF4
void I2C_Pin_Config(void) {
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
}

// Configure speed, frequency, rise time, and enable I2C1
void I2C_Peripheral_Config(void) {
    // Clear the FREQ[5:0] bits (lower 6 bits) to safely overwrite the frequency configuration.
    I2C1->CR2 &= ~(63 << 0);

    // Set the internal peripheral clock frequency to 16MHz (Matches the APB1 bus speed).
    // This value is critical for the internal hardware timers to accurately calculate I2C timings.
    I2C1->CR2 |= (16 << 0);

    // Clear the 11-bit CCR[10:0] mask to reset the clock control register field.
    I2C1->CCR &= ~(2047 << 0);

    // Configure the Clock Control Register for Standard Mode (100kHz).
    // Formula: CCR = T_high / T_pclk1 -> For 100kHz (10us period), T_high = 5us. 5us / (1 / 16MHz) = 80.
    I2C1->CCR |= (80 << 0);

    // Clear the 6-bit TRISE[5:0] mask to safely write the maximum allowed rise time.
    I2C1->TRISE &= ~(63 << 0);
    
    // Set the maximum allowed t_rise for Standard Mode (Max = 1000ns / 1us).
    // Formula: TRISE = (Max_Rise_Time / T_pclk1) + 1 -> (1000ns / 62.5ns) + 1 = 16 + 1 = 17.
    I2C1->TRISE |= (17 << 0);

    // Fire peripheral power configurations by setting the PE (Peripheral Enable) bit high.
    // The I2C1 hardware engine is now alive and actively driving the physical bus lanes.
    I2C1->CR1 |= (1 << 0);
}
