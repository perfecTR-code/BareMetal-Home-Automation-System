#include "nRF24.h"
#include "Drivers/SPI.h"

// Write a single 8-bit value to a configuration register
void nRF24_Write_Register(uint8_t reg, uint8_t value) {
    CSN_LOW();
    SPI_Send_Data(NRF24_CMD_W_REGISTER | reg);  // Send write command + register address
    SPI_Send_Data(value);                        // Send the data byte
    CSN_HIGH();
}

// Read a single 8-bit value from a configuration register
uint8_t nRF24_Read_Register(uint8_t reg) {
    CSN_LOW();
    SPI_Send_Data(NRF24_CMD_R_REGISTER | reg);  // Send read command + register address
    uint8_t result = SPI_Receive_Data();         // Transmit 0xFF dummy byte, receive register data
    CSN_HIGH();
    return result;
}

// Read the current STATUS register of the transceiver
uint8_t nRF24_Read_Status(void) {
    CSN_LOW();
    uint8_t status = SPI_Receive_Data();        // Sending 0xFF NOP shifts out the STATUS byte
    CSN_HIGH();
    return status;
}

// Clear all active interrupt/notification flags in STATUS register
void nRF24_Clear_Flags(void) {
    // Write 1 to bits 6, 5, 4 to clear RX_DR, TX_DS, and MAX_RT flags
    nRF24_Write_Register(NRF24_REG_STATUS, 0x70);
}

// Configure the transceiver into Transmitter (TX) mode
void nRF24_Set_Mode_TX(void) {
    CE_LOW(); // Turn off antenna before making configuration changes
    nRF24_Write_Register(NRF24_REG_CONFIG, NRF24_CONFIG_TX); // Clear PRIM_RX to 0
}

// Configure the transceiver into Receiver (RX) mode and activate listening
void nRF24_Set_Mode_RX(void) {
    nRF24_Write_Register(NRF24_REG_CONFIG, NRF24_CONFIG_RX); // Set PRIM_RX to 1
    CE_HIGH(); // Activate antenna to start listening over the air
}

// Send a data packet over the air
void nRF24_Transmit(uint8_t *data, uint8_t size) {
    CSN_LOW();
    SPI_Send_Data(NRF24_CMD_W_TX_PAYLOAD); // Send write TX payload command
    for (uint8_t i = 0; i < size; i++) {
        SPI_Send_Data(data[i]);            // Shift out each byte of the payload
    }
    CSN_HIGH();

    // Generate at least 10 us high pulse on CE to trigger the RF transmitter hardware
    CE_HIGH();
    Delay_us(15); // Wait 15 us to guarantee the transmission trigger
    CE_LOW();     // Put the transceiver back to Standby-I mode
}

// Set the 5-byte target transmit address (TX_ADDR)
void nRF24_Set_TX_Address(uint8_t *address) {
    CSN_LOW();
    SPI_Send_Data(NRF24_CMD_W_REGISTER | NRF24_REG_TX_ADDR); // Write to TX_ADDR register (0x10)
    for (int i = 0; i < 5; i++) {
        SPI_Send_Data(address[i]); // Stream all 5 bytes of the target address consecutively
    }
    CSN_HIGH();
}

// Set the 5-byte local receive address (RX_ADDR_P0)
void nRF24_Set_RX_Address(uint8_t *address) {
    CSN_LOW();
    SPI_Send_Data(NRF24_CMD_W_REGISTER | NRF24_REG_RX_ADDR_P0); // Write to RX_ADDR_P0 register (0x0A)
    for (int i = 0; i < 5; i++) {
        SPI_Send_Data(address[i]); // Stream all 5 bytes of the local address consecutively
    }
    CSN_HIGH();
}

// Complete initialization sequence of the nRF24L01+ transceiver
void nRF24_Init(void) {
    // 1. Configure the transceiver as a Receiver and power it up
    nRF24_Write_Register(NRF24_REG_CONFIG, NRF24_CONFIG_RX);

    // 2. Clear any leftover interrupt flags to start clean
    nRF24_Clear_Flags();

    // 3. Set the communication RF channel to 100 (2.500 GHz) to bypass WiFi noise
    nRF24_Write_Register(NRF24_REG_RF_CH, 0x64);

    // 4. Start active listening over the air
    CE_HIGH();
}

// Final packet sending function (TX Mode -> Transmit -> Wait -> RX Mode)
void nRF24_Send_Packet(uint8_t *data, uint8_t size) {
    // nRF24L01+ max payload size limit check (32 bytes = 256 bits)
    if (size > 32) {
        size = 32; // Limit size to 32 bytes to prevent buffer overflow
    }

    // 1. Switch transceiver to Transmitter (TX) Mode
    nRF24_Set_Mode_TX();

    // 2. Load the payload and pulse CE to trigger transmission
    nRF24_Transmit(data, size);

    // 3. Wait untilfs the transmission is complete (either success or max retries error)
    uint8_t status = 0;
    while (1) {
        status = nRF24_Read_Status();
        // Check if TX_DS (bit 5) or MAX_RT (bit 4) is set in STATUS
        if ((status & (1 << 5)) || (status & (1 << 4))) {

            break; // Sinyal havaya fırlatıldı veya hata ile sonuçlandı
        }
    }

    // 4. Clear the interrupt flags so they don't block future communication
    nRF24_Clear_Flags();

    // 5. Switch transceiver back to Receiver (RX) Mode to start listening again
    nRF24_Set_Mode_RX();
}
