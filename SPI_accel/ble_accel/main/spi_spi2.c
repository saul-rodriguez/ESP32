#include "spi_spi2.h"

spi_device_handle_t spi2;

void SPI_init(void)
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
		        .clock_speed_hz = 10000000,  // 1 MHz
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
 * SPI_write_reg()
 * Write a byte to a slave device.
 * add = 8 bit address
 * reg = 8 bit register
 * value = 8 bit value
 */
void SPI_write_reg(uint8_t add, uint8_t reg, uint8_t value)
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

/*
 * SPI_read_reg()
 * Reads a byte from a SPI slave
 * add = address + read bit (user must set read bit before)
 * reg = register
 * value = pointer to 8-bit variable to store value.
 */

void SPI_read_reg(uint8_t add, uint8_t reg, uint8_t *value)
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

void SPI_transaction(uint8_t* pData, uint16_t Size)
{
	spi_transaction_t t = {
		.length = Size * 8,
		.rxlength = Size * 8,
		.tx_buffer = pData,
		.rx_buffer = pData
		//.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA
	};

	ESP_ERROR_CHECK(spi_device_polling_transmit(spi2, &t));
}

/*
void SPI_receive(uint8_t* pData, uint16_t Size)
{

}

void SPI_transmit(uint8_t* pData, uint16_t Size)
{

}
*/


//write function for ST devices
int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
	//TODO: create a buffer data, with data[0] = reg, and data[1] = bufp[0], ...
	// Send len+1 bytes by using SPI_transaction
	uint8_t buffer[16];
	uint8_t i,aux;

	buffer[0] = reg;

	aux = 1;
	for (i = 0; i < len; i++) {
		buffer[aux] = bufp[i];
		aux++;
	}

	SPI_transaction(buffer, aux);

	return 0;
}


//read function for ST devices
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
	//TODO: create a buffer data, with data[0] = reg | 0x80, and data[1] = bufp[0], ...
		// Send len+1 bytes by using SPI_transaction

	uint8_t buffer[16];
	uint8_t i,aux;

	buffer[0] = reg | 0x80; //set write bit for ST devices!

	aux = 1;
	for (i = 0; i < len; i++) {
		buffer[aux] = 0x00;
		aux++;
	}

	SPI_transaction(buffer, aux);

	aux = 1;
	for (i = 0; i < len; i++) {
			//buffer[aux] = 0x00;
			bufp[i] = buffer[aux];
			aux++;
	}

	return 0;
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *

static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len)
{
#if defined(NUCLEO_F411RE)
  HAL_I2C_Mem_Write(handle, ISM330DHCX_I2C_ADD_H, reg,
                    I2C_MEMADD_SIZE_8BIT, (uint8_t*) bufp, len, 1000);
#elif defined(STEVAL_MKI109V3)
  HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(handle, &reg, 1, 1000);
  HAL_SPI_Transmit(handle, (uint8_t*) bufp, len, 1000);
  HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_SET);
#elif defined(SPC584B_DIS)
  i2c_lld_write(handle,  ISM330DHCX_I2C_ADD_H & 0xFE, reg,
               (uint8_t*) bufp, len);
#endif
  return 0;
}
*/

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *

static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len)
{
#if defined(NUCLEO_F411RE)
  HAL_I2C_Mem_Read(handle, ISM330DHCX_I2C_ADD_H, reg,
                   I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
#elif defined(STEVAL_MKI109V3)
  reg |= 0x80;
  HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(handle, &reg, 1, 1000);
  HAL_SPI_Receive(handle, bufp, len, 1000);
  HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_SET);
#elif defined(SPC584B_DIS)
  i2c_lld_read(handle, ISM330DHCX_I2C_ADD_H & 0xFE, reg, bufp, len);
#endif
  return 0;
}

*/

