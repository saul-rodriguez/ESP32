#ifndef SPI_SPI2_H
#define	SPI_SPI2_H

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"

//HSPI- SPI2 Default GIO pins
#define CLK_PIN     14
#define MOSI_PIN    13
#define MISO_PIN	12
#define CS_PIN      15

extern spi_device_handle_t spi2;

void SPI_init(void);
void SPI_write_reg(uint8_t add, uint8_t reg, uint8_t value);
void SPI_read_reg(uint8_t add, uint8_t reg, uint8_t *value);
//void SPI_transmit(uint8_t* pData, uint16_t Size);
//void SPI_receive(uint8_t* pData, uint16_t Size);
void SPI_transaction(uint8_t* pData, uint16_t Size);
int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len);
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);


#endif /* SPI_SPI2_H*/
