#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "mcp23s17.h"

//HSPI- SPI2 Default GIO pins
//#define CLK_PIN     14
//#define MOSI_PIN    13
//#define MISO_PIN	12
//#define CS_PIN      15

//SPI device handler
//spi_device_handle_t spi2;

/*
void spi_init()
{
	esp_err_t ret;

	//Create a spi bus configuration and initialize it
	spi_bus_config_t buscfg={
	        .miso_io_num = MISO_PIN,
	        .mosi_io_num = MOSI_PIN,
	        .sclk_io_num = CLK_PIN,
	        .quadwp_io_num = -1,
	        .quadhd_io_num = -1,
	        .max_transfer_sz = 32,
	    };

	ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
	ESP_ERROR_CHECK(ret);

	//Configure the device
	spi_device_interface_config_t devcfg={
	        .clock_speed_hz = 1000000,  // 1 MHz
	        .mode = 0,                  //SPI mode 0
	        .spics_io_num = CS_PIN,
	        .queue_size = 1,
	        //.flags = SPI_DEVICE_HALFDUPLEX,
	        .pre_cb = NULL,
	        .post_cb = NULL,
	    };
	ret = spi_bus_add_device(SPI2_HOST, &devcfg, &spi2);
	ESP_ERROR_CHECK(ret);

}

void write_reg(uint8_t reg, uint8_t value) {
   uint8_t tx_data[4] = { reg, value };
   //uint8_t rx_data[4];
   spi_transaction_t t = {

       .tx_buffer = tx_data,
       .length = 2 * 8,
	   .rxlength = 2 * 8,
	 //  .rx_buffer = rx_data,
	   .flags = SPI_TRANS_USE_RXDATA

   };
   ESP_ERROR_CHECK(spi_device_polling_transmit(spi2, &t));

   ESP_LOGI("SPI", "This is an info %x", t.rx_data[0]);
}

*/

void app_main(void)
{
	//uint8_t a;
	//esp_err_t ret;

	SPI_init();

	MCP23S17_setAddress(0x20);

	MCP23S17_setTrisA(0x7f); // Only A7 activated as output
	MCP23S17_writePortA(0x00);

	MCP23S17_setTrisB(0xff);

/*
	while(1) {
		vTaskDelay(100/portTICK_PERIOD_MS);
		MCP23S17_writePortA(0xff);
		vTaskDelay(100/portTICK_PERIOD_MS);
		MCP23S17_writePortA(0x00);
		//MCP23S17_transaction();
		//MCP23S17_readPortB();
	}
*/
	/*
	while(1) {
		MCP23S17_transaction();
	}
	*/

	while(1) {
			MCP23S17_platform_write();
		}
}
