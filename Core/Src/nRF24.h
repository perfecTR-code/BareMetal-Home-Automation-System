#ifndef SRC_NRF24_H_
#define SRC_NRF24_H_

#include "stm32f4xx.h"
#include <stdint.h>

// --- nRF24L01+ Register Addresses ---
#define NRF24_REG_CONFIG     0x00   // Configuration register
#define NRF24_REG_EN_AA      0x01   // Enable Auto Acknowledgment
#define NRF24_REG_RF_CH      0x05   // RF channel register
#define NRF24_REG_STATUS     0x07   // Status register
#define NRF24_REG_RX_ADDR_P0 0x0A   // Receive address data pipe 0 (5 bytes)
#define NRF24_REG_TX_ADDR    0x10   // Transmit address (5 bytes)

// --- nRF24L01+ SPI Command Set ---
#define NRF24_CMD_W_REGISTER 0x20   // Write register command mask
#define NRF24_CMD_R_REGISTER 0x00   // Read register command mask
#define NRF24_CMD_W_TX_PAYLOAD 0xA0 // Write TX payload command
#define NRF24_CMD_R_RX_PAYLOAD 0x61 // Read RX payload command
#define NRF24_CMD_FLUSH_TX   0xE1   // Flush TX FIFO command
#define NRF24_CMD_FLUSH_RX   0xE2   // Flush RX FIFO command
#define NRF24_CMD_NOP        0xFF   // No operation command

// --- CONFIG register bit values ---
#define NRF24_CONFIG_RX      0x0F   // Power Up + Receiver Mode (0000 1111)
#define NRF24_CONFIG_TX      0x0E   // Power Up + Transmitter Mode (0000 1110)

// --- Function prototypes ---
void nRF24_Init(void);
void nRF24_Write_Register(uint8_t reg, uint8_t value);
uint8_t nRF24_Read_Register(uint8_t reg);
uint8_t nRF24_Read_Status(void);
void nRF24_Clear_Flags(void);
void nRF24_Set_Mode_TX(void);
void nRF24_Set_Mode_RX(void);
void nRF24_Transmit(uint8_t *data, uint8_t size);

// 5-byte address configuration functions
void nRF24_Set_TX_Address(uint8_t *address);
void nRF24_Set_RX_Address(uint8_t *address);

// Final packet sending function (TX Mode -> Transmit -> Wait -> RX Mode)
void nRF24_Send_Packet(uint8_t *data, uint8_t size);

#endif /* SRC_NRF24_H_ */
