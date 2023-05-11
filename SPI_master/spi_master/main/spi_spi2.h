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

void spi_init(void);
void write_reg(uint8_t add, uint8_t reg, uint8_t value);
void read_reg(uint8_t add, uint8_t reg, uint8_t *value);

#endif /* SPI_SPI2_H*/
