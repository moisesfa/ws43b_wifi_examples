#pragma once

/**
 * @brief I2C address of the ch422g
 *
 * And the 7-bit slave address is the most important data for users.
 * For example, if a chip's A0,A1,A2 are connected to GND, it's 7-bit slave address is 1001000b(0x48).
 * Then users can use `ESP_IO_EXPANDER_I2C_CH422G_ADDRESS_000` to init it.
 */
#define ESP_IO_EXPANDER_I2C_CH422G_ADDRESS_000    (0x24)
#define GPIO_INPUT_IO_4 4 

// Extender Pin define
#define TP_RST  1
#define LCD_BL  2
#define LCD_RST 3
#define SD_CS   4
#define USB_SEL 5

// I2C Pin define
#define I2C_MASTER_NUM 0
#define I2C_MASTER_SDA_IO 8
#define I2C_MASTER_SCL_IO 9



void expander_init(void);