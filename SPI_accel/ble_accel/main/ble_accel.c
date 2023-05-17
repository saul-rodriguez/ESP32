#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "spi_spi2.h"
#include "ism330dhcx_reg.h"

/* Private variables ---------------------------------------------------------*/
static int16_t data_raw_acceleration[3];
static int16_t data_raw_angular_rate[3];
//static int16_t data_raw_temperature;
static float acceleration_mg[3];
static float angular_rate_mdps[3];
//static float temperature_degC;
static uint8_t whoamI, rst;
//static uint8_t tx_buffer[1000];

#define    BOOT_TIME          20 //ms

uint8_t pause();

void app_main(void)
{
	uint8_t option;
	stmdev_ctx_t dev_ctx;

	  /* Initialize mems driver interface */
	dev_ctx.write_reg = platform_write;
	dev_ctx.read_reg = platform_read;

	SPI_init();

	// Boot delay
	vTaskDelay(BOOT_TIME/portTICK_PERIOD_MS);

	/* Check device ID */
	ism330dhcx_device_id_get(&dev_ctx, &whoamI);
	ESP_LOGI("ISM", "whoiam %x", whoamI);

	/* Restore default configuration */
    ism330dhcx_reset_set(&dev_ctx, PROPERTY_ENABLE);
    do {
        ism330dhcx_reset_get(&dev_ctx, &rst);
        ESP_LOGI("ISM", "resetting dev %x", rst);
    } while (rst);

    option = pause();

    /* Start device configuration. */
      ism330dhcx_device_conf_set(&dev_ctx, PROPERTY_ENABLE);
      /* Enable Block Data Update */
      ism330dhcx_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
      /* Set Output Data Rate */
      ism330dhcx_xl_data_rate_set(&dev_ctx, ISM330DHCX_XL_ODR_104Hz);
      ism330dhcx_gy_data_rate_set(&dev_ctx, ISM330DHCX_GY_ODR_104Hz);
      /* Set full scale */
      ism330dhcx_xl_full_scale_set(&dev_ctx, ISM330DHCX_16g);
      ism330dhcx_gy_full_scale_set(&dev_ctx, ISM330DHCX_125dps);
      /* Configure filtering chain(No aux interface)
       *
       * Accelerometer - LPF1 + LPF2 path
       */
      ism330dhcx_xl_hp_path_on_out_set(&dev_ctx, ISM330DHCX_LP_ODR_DIV_100);
      ism330dhcx_xl_filter_lp2_set(&dev_ctx, PROPERTY_ENABLE);

      //ism330dhcx_g
      ism330dhcx_gy_lp1_bandwidth_set(&dev_ctx, ISM330DHCX_MEDIUM);
      ism330dhcx_gy_filter_lp1_set(&dev_ctx, PROPERTY_ENABLE);


      /* Read samples in polling mode (no int) */
	while(1) {
		 uint8_t reg;

		    /* Read output only if new xl value is available */
		 switch (option) {
		 	 	 	 case	'a':
		 	 	 		 	 	 ism330dhcx_xl_flag_data_ready_get(&dev_ctx, &reg);

		 	 	 		 	 	 if (reg) {
		 	 	 		 	 		 // Read acceleration field data
		 	 	 		 	 		 memset(data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
		 	 	 		 	 		 ism330dhcx_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
		 	 	 		 	 		 acceleration_mg[0] = ism330dhcx_from_fs2g_to_mg(data_raw_acceleration[0]);
		 	 	 		 	 		 acceleration_mg[1] = ism330dhcx_from_fs2g_to_mg(data_raw_acceleration[1]);
		 	 	 		 	 		 acceleration_mg[2] = ism330dhcx_from_fs2g_to_mg(data_raw_acceleration[2]);
		 	 	 		 	 		 //sprintf((char *)tx_buffer,"Acceleration [mg]:%4.2f\t%4.2f\t%4.2f\r\n", acceleration_mg[0], acceleration_mg[1], acceleration_mg[2]);
		 	 	 		 	 		 printf("Accel [mg]:%4.2f\t%4.2f\t%4.2f\r\n", acceleration_mg[0], acceleration_mg[1], acceleration_mg[2]);
		 	 	 		 	 		 //tx_com(tx_buffer, strlen((char const *)tx_buffer));
		 	 	 		 	 	 }
		 	 	 		 	 	 break;
		 	 	 	 case 'g':
		 	 	 		 	 	 ism330dhcx_gy_flag_data_ready_get(&dev_ctx, &reg);

		 	 	 				 if (reg) {
		 	 	 				      // Read angular rate field data
		 	 	 				      memset(data_raw_angular_rate, 0x00, 3 * sizeof(int16_t));
		 	 	 				      ism330dhcx_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate);
		 	 	 				      angular_rate_mdps[0] = ism330dhcx_from_fs2000dps_to_mdps(data_raw_angular_rate[0]);
		 	 	 				      angular_rate_mdps[1] = ism330dhcx_from_fs2000dps_to_mdps(data_raw_angular_rate[1]);
		 	 	 				      angular_rate_mdps[2] = ism330dhcx_from_fs2000dps_to_mdps(data_raw_angular_rate[2]);
		 	 	 				      printf("Angrate [mdps]:%4.2f\t%4.2f\t%4.2f\r\n", angular_rate_mdps[0], angular_rate_mdps[1], angular_rate_mdps[2]);
		 	 	 				         //tx_com(tx_buffer, strlen((char const *)tx_buffer));
		 	 	 				 }
		 	 	 				 break;


		 	 	 	 default: 	break;
		 }

		 vTaskDelay(25/portTICK_PERIOD_MS);

	}

}

uint8_t pause()
{
	char character = 0;
	char str[100];
	memset(str,0,sizeof(str));

	printf("Enter a (accel) or g (gyro) + ENTER");

	while(character!= '\n')
	  {
	    character =getchar();
	    if(character!= 0xff)
	    {
	      str[strlen(str)] = character;
	      printf("%c",character);
	    }
	    vTaskDelay(100 /portTICK_PERIOD_MS);
	  }

	return str[0];
	 //printf("User Entered: %s\n", str);
}


