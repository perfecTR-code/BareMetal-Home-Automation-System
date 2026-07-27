#include "nRF24.h"
#include "Drivers/SPI.h"

/*
  ==============================================================================
  nRF24L01+ Bare-Metal Driver Implementation for STM32F411RE
  
  Datasheet Reference: Nordic Semiconductor nRF24L01+ Product Specification
  ==============================================================================
*/

/**
 * @brief  nRF24L01+ Register'ına 1 Byte Veri Yazma Fonksiyonu.
 * @param  reg: Register adresi (0x00 - 0x1D)
 * @param  value: Yazılacak veri (1 byte)
 * @note   Datasheet Ref: Section 8.1 "SPI Commands", Table 16 (Sayfa 51)
 *         Komut Yapısı: 0x20 | (reg & 0x1F)
 */
void nRF24_Write_Register(uint8_t reg, uint8_t value) {
    CSN_LOW();
    SPI_Tranceive(NRF24_CMD_W_REGISTER | (reg & 0x1F));
    SPI_Tranceive(value);
    CSN_HIGH();
}

/**
 * @brief  nRF24L01+ Register'ından 1 Byte Veri Okuma Fonksiyonu.
 * @param  reg: Register adresi (0x00 - 0x1D)
 * @return Okunan register değeri
 * @note   Datasheet Ref: Section 8.1 "SPI Commands", Table 16 (Sayfa 51)
 *         Komut Yapısı: 0x00 | (reg & 0x1F)
 */
uint8_t nRF24_Read_Register(uint8_t reg) {
    CSN_LOW();
    SPI_Tranceive(NRF24_CMD_R_REGISTER | (reg & 0x1F));
    uint8_t value = SPI_Tranceive(NRF24_CMD_NOP); // 0xFF yollayarak MISO'dan gelen değeri okuyoruz
    CSN_HIGH();
    return value;
}

/**
 * @brief  nRF24L01+'e Çoklu Byte (Buffer) Yazma (Adresler ve Paketler için).
 * @param  reg: SPI Komut veya Register Yazma Maskesi
 * @param  pBuf: Gönderilecek veri dizisi (pointer)
 * @param  bytes: Byte sayısı
 */
void nRF24_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes) {
    CSN_LOW();
    SPI_Tranceive(reg);
    for (uint8_t i = 0; i < bytes; i++) {
        SPI_Tranceive(pBuf[i]);
    }
    CSN_HIGH();
}

/**
 * @brief  nRF24L01+'den Çoklu Byte (Buffer) Okuma (Gelen Paketler için).
 * @param  reg: SPI Komut veya Register Okuma Maskesi
 * @param  pBuf: Okunan verilerin yazılacağı dizi (pointer)
 * @param  bytes: Byte sayısı
 */
void nRF24_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes) {
    CSN_LOW();
    SPI_Tranceive(reg);
    for (uint8_t i = 0; i < bytes; i++) {
        pBuf[i] = SPI_Tranceive(NRF24_CMD_NOP);
    }
    CSN_HIGH();
}

/**
 * @brief  STATUS Register'ını okur.
 * @return STATUS register değeri
 */
uint8_t nRF24_Read_Status(void) {
    CSN_LOW();
    uint8_t status = SPI_Tranceive(NRF24_CMD_NOP);
    CSN_HIGH();
    return status;
}

/**
 * @brief  Kesme Bayraklarını (RX_DR, TX_DS, MAX_RT) Temizler.
 * @note   Datasheet Ref: STATUS Register (Sayfa 56). Bit 6,5,4 alanına '1' yazmak temizler (0x70).
 */
void nRF24_Clear_Flags(void) {
    nRF24_Write_Register(NRF24_REG_STATUS, 0x70);
}

/**
 * @brief  TX FIFO Belleğini Temizler.
 */
void nRF24_Flush_TX(void) {
    CSN_LOW();
    SPI_Tranceive(NRF24_CMD_FLUSH_TX);
    CSN_HIGH();
}

/**
 * @brief  RX FIFO Belleğini Temizler.
 */
void nRF24_Flush_RX(void) {
    CSN_LOW();
    SPI_Tranceive(NRF24_CMD_FLUSH_RX);
    CSN_HIGH();
}

/**
 * @brief  Modülü Verici (TX) Moduna Alır.
 * @note   Datasheet Ref: Section 6.1.4 "TX Mode" (Sayfa 22)
 *         PRIM_RX (BIT 0) = 0, PWR_UP (BIT 1) = 1 yapılması gerekir.
 */
void nRF24_Set_Mode_TX(void) {
    CE_LOW();
    uint8_t config = nRF24_Read_Register(NRF24_REG_CONFIG);
    config &= ~(1 << 0); // PRIM_RX = 0 (TX Mode)
    config |=  (1 << 1); // PWR_UP = 1
    nRF24_Write_Register(NRF24_REG_CONFIG, config);
    Delay_us(130);       // Standby-I geçiş gecikmesi 130µs (Datasheet Sayfa 24, Table 13)
}

/**
 * @brief  Modülü Alıcı (RX) Moduna Alır.
 * @note   Datasheet Ref: Section 6.1.3 "RX Mode" (Sayfa 22)
 *         PRIM_RX (BIT 0) = 1, PWR_UP (BIT 1) = 1 yapılması gerekir.
 */
void nRF24_Set_Mode_RX(void) {
    CE_LOW();
    uint8_t config = nRF24_Read_Register(NRF24_REG_CONFIG);
    config |= (1 << 0);  // PRIM_RX = 1 (RX Mode)
    config |= (1 << 1);  // PWR_UP = 1
    nRF24_Write_Register(NRF24_REG_CONFIG, config);
    CE_HIGH();           // Dinleme modunu başlatmak için CE HIGH yapılır
    Delay_us(130);       // Oturma süresi 130µs
}

/**
 * @brief  5-byte Verici Adresini (TX_ADDR) ve Pipe 0 Alıcı Adresini ayarlar.
 * @param  address: 5 byte'lık adres dizisi
 */
void nRF24_Set_TX_Address(uint8_t *address) {
    nRF24_Write_Buf(NRF24_CMD_W_REGISTER | NRF24_REG_TX_ADDR, address, 5);
    nRF24_Write_Buf(NRF24_CMD_W_REGISTER | NRF24_REG_RX_ADDR_P0, address, 5); // Auto-ACK için şarttır
}

/**
 * @brief  5-byte Alıcı Adresini (RX_ADDR_P0) ayarlar.
 * @param  address: 5 byte'lık adres dizisi
 */
void nRF24_Set_RX_Address(uint8_t *address) {
    nRF24_Write_Buf(NRF24_CMD_W_REGISTER | NRF24_REG_RX_ADDR_P0, address, 5);
}

/**
 * @brief  nRF24L01+ Donanımını İlk Yapılandırma Fonksiyonu.
 * @note   Adres Genişliği: 5 Byte, Kanal: 76 (2476MHz), Hız: 2Mbps, Güç: 0dBm.
 */
void nRF24_Init(void) {
    CE_LOW();
    CSN_HIGH();
    Delay_us(5000); // Power-On Reset için 5ms bekleme

    nRF24_Flush_TX();
    nRF24_Flush_RX();
    nRF24_Clear_Flags();

    // 1. Adres Uzunluğu = 5 Byte (SETUP_AW = 0x03) (Datasheet Sayfa 55)
    nRF24_Write_Register(NRF24_REG_SETUP_AW, 0x03);

    // 2. Otomatik Tekrar Gönderme = 750µs gecikme, 15 tekrar (SETUP_RETR = 0x2F) (Datasheet Sayfa 55)
    nRF24_Write_Register(NRF24_REG_SETUP_RETR, 0x2F);

    // 3. RF Kanalı = Kanal 76 (2476 MHz - WiFi çakışmalarını önler) (Datasheet Sayfa 55)
    nRF24_Write_Register(NRF24_REG_RF_CH, 76);

    // 4. Veri Hızı = 2Mbps, Yayın Gücü = 0dBm (RF_SETUP = 0x0E) (Datasheet Sayfa 55)
    nRF24_Write_Register(NRF24_REG_RF_SETUP, 0x0E);

    // 5. Pipe 0 için Auto ACK Aktif (EN_AA = 0x01) (Datasheet Sayfa 54)
    nRF24_Write_Register(NRF24_REG_EN_AA, 0x01);

    // 6. Pipe 0 Alıcı Adresini Aç (EN_RXADDR = 0x01) (Datasheet Sayfa 54)
    nRF24_Write_Register(NRF24_REG_EN_RXADDR, 0x01);

    // 7. Pipe 0 Paket Boyutu = 32 Byte (RX_PW_P0 = 32) (Datasheet Sayfa 57)
    nRF24_Write_Register(NRF24_REG_RX_PW_P0, 32);

    // 8. 2-byte CRC, Power Up, Varsayılan TX Modu (CONFIG = 0x0E) (Datasheet Sayfa 54)
    nRF24_Write_Register(NRF24_REG_CONFIG, 0x0E);

    Delay_us(1500); // Standby-I moduna geçiş süresi 1.5ms (Datasheet Sayfa 24, Table 13)
}

/**
 * @brief  Veriyi Havaya Fırlatma (Transmit) Fonksiyonu.
 * @param  data: Gönderilecek veri dizisi
 * @param  size: Veri boyutu (maks 32 byte)
 * @note   CE pini en az 10µs süresince HIGH çekilip düşürülür.
 *         Datasheet Ref: Section 6.1.4 (Sayfa 22)
 */
void nRF24_Transmit(uint8_t *data, uint8_t size) {
    CE_LOW();
    nRF24_Flush_TX();
    nRF24_Clear_Flags();

    // Veriyi TX FIFO belleğine yaz
    nRF24_Write_Buf(NRF24_CMD_W_TX_PAYLOAD, data, size);

    // CE pinini en az 10µs HIGH çekerek RF yayınını başlat
    CE_HIGH();
    Delay_us(15);
    CE_LOW();
}

/**
 * @brief  Paket Gönderme Ana Fonksiyonu (TX Moduna Alır ve Gönderir).
 * @param  data: Gönderilecek veri dizisi
 * @param  size: Veri boyutu
 */
void nRF24_Send_Packet(uint8_t *data, uint8_t size) {
    nRF24_Set_Mode_TX();
    nRF24_Transmit(data, size);
}
