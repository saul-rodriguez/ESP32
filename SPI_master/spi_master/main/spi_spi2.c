#include "spi_spi2.h"

spi_device_handle_t spi2;

void spi_init(void)
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

/*
void write_reg(uint8_t add, uint8_t reg, uint8_t value)
{
	 uint8_t tx_data[4] = { add, reg, value };
	   //uint8_t rx_data[4];  //Not necessary if the rx_data buffer in the spi_transaction structure is used (SPI_TRANS_USE_RXDATA).
	 spi_transaction_t t = {
		.tx_buffer = tx_data,
		.length = 3 * 8,
		.rxlength = 3 * 8,
		//  .rx_buffer = rx_data,
		.flags = SPI_TRANS_USE_RXDATA
	 };

	 ESP_ERROR_CHECK(spi_device_polling_transmit(spi2, &t));
	 //ESP_LOGI("SPI", "SPI RX %x", t.rx_data[0]);

}
*/

void write_reg(uint8_t add, uint8_t reg, uint8_t value)
{
	 spi_transaction_t t = {
		//.tx_buffer = tx_data,
		.length = 3 * 8,
		.rxlength = 3 * 8,
		//  .rx_buffer = rx_data,
		.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA
	 };

	 t.tx_data[0] = add;
	 t.tx_data[1] = reg;
	 t.tx_data[2] = value;

	 ESP_ERROR_CHECK(spi_device_polling_transmit(spi2, &t));
}


void read_reg(uint8_t add, uint8_t reg, uint8_t *value)
{
	//uint8_t tx_data[4] = { add, reg, 0x00 };
	//tx_data[0] |= 0x01; // Set read bit

	spi_transaction_t t = {
		//.tx_buffer = tx_data,
		.length = 3 * 8,
		.rxlength = 3 * 8,
		.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA
	};

	t.tx_data[0] = add; //read bit set
	t.tx_data[1] = reg;
	t.tx_data[2] = 0; // dummy data

	ESP_ERROR_CHECK(spi_device_polling_transmit(spi2, &t));

	*value = t.rx_data[2];

	ESP_LOGI("SPI", "SPI RX %x %x %x", t.rx_data[0], t.rx_data[1], t.rx_data[2] );

}
