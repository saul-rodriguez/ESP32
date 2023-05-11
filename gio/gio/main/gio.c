
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN 32
#define INT_PIN 2

void app_main(void)
{
	
  //Reset an gpio to default state 
  //(select gpio function, enable pullup and disable input and output).
  gpio_reset_pin(LED_PIN); 
  gpio_reset_pin(INT_PIN); 
    
  gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
  gpio_set_direction(INT_PIN, GPIO_MODE_INPUT);
	
	int ON = 0;
	/*
	while(true) {
		ON = !ON;
		gpio_set_level(LED_PIN, ON);
		vTaskDelay(1000/ portTICK_PERIOD_MS);		
	} */
	
	while(true) {
		if (gpio_get_level(INT_PIN) == 1) {
			gpio_set_level(LED_PIN,1);
		} else {
			gpio_set_level(LED_PIN,0);
		}
		vTaskDelay(1);
	}
}


