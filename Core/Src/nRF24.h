#ifndef SRC_NRF24_H_
#define SRC_NRF24_H_

#include "stm32f4xx.h"
#include <stdint.h>

// --- nRF24L01+ Register Addresses ---
#define NRF24_REG_CONFIG     0x00   // Configuration register (Datasheet Page 54)
#define NRF24_REG_EN_AA      0x01   // Enable Auto Acknowledgment (Datasheet Page 54)
#define NRF24_REG_EN_RXADDR  0x02   // Enable RX addresses (Datasheet Page 54)
#define NRF24_REG_SETUP_AW   0x03   // Setup Address Widths (Datasheet Page 55)
#define NRF24_REG_SETUP_RETR 0x04   // Setup Auto Retransmission (Datasheet Page 55)
#define NRF24_REG_RF_CH      0x05   // RF channel register (Datasheet Page 55)
#define NRF24_REG_RF_SETUP   0x06   // RF setup register (Datasheet Page 55)
#define NRF24_REG_STATUS     0x07   // Status register (Datasheet Page 56)
#define NRF24_REG_OBSERVE_TX 0x08   // Transmit observe register (Datasheet Page 56)
#define NRF24_REG_RPD        0x09   // Received Power Detector (Datasheet Page 56)
#define NRF24_REG_RX_ADDR_P0 0x0A   // Receive address data pipe 0 (5 bytes) (Datasheet Page 57)
#define NRF24_REG_TX_ADDR    0x10   // Transmit address (5 bytes) (Datasheet Page 57)
#define NRF24_REG_RX_PW_P0   0x11   // RX payload width pipe 0 (Datasheet Page 57)
#define NRF24_REG_FIFO_STATUS 0x17  // FIFO status register (Datasheet Page 58)

// --- nRF24L01+ SPI Command Set (Datasheet Table 16, Page 51) ---
#define NRF24_CMD_R_REGISTER 0x00   // Read register command mask (000A AAAA)
#define NRF24_CMD_W_REGISTER 0x20   // Write register command mask (001A AAAA)
#define NRF24_CMD_R_RX_PAYLOAD 0x61 // Read RX payload command
#define NRF24_CMD_W_TX_PAYLOAD 0xA0 // Write TX payload command
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
void nRF24_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);
void nRF24_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);
uint8_t nRF24_Read_Status(void);
void nRF24_Clear_Flags(void);
void nRF24_Flush_TX(void);
void nRF24_Flush_RX(void);
void nRF24_Set_Mode_TX(void);
void nRF24_Set_Mode_RX(void);
void nRF24_Transmit(uint8_t *data, uint8_t size);

// 5-byte address configuration functions
void nRF24_Set_TX_Address(uint8_t *address);
void nRF24_Set_RX_Address(uint8_t *address);

// Final packet sending function (TX Mode -> Transmit -> Wait -> RX Mode)
void nRF24_Send_Packet(uint8_t *data, uint8_t size);

#endif /* SRC_NRF24_H_ */
