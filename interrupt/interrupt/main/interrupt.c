#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"

#define ESP_INR_FLAG_DEFAULT 0
#define LED_PIN  32
#define PUSH_BUTTON_PIN1  34
#define PUSH_BUTTON_PIN2  35

TaskHandle_t ISR = NULL;
volatile uint32_t gpio_num;

void IRAM_ATTR isr_handler(void *arg){ // arg is either PUSH_BUTTON_PIN1 or PUSH_BUTTON_PIN2
	//uint32_t gpio_num = (uint32_t) arg;
	gpio_num = (uint32_t) arg; 
	xTaskResumeFromISR(ISR);
}

void interrupt_task(void *arg){
  bool led_status = false;
  while(1){
    vTaskSuspend(NULL);
    led_status = !led_status;
    gpio_set_level(LED_PIN, led_status);
    if (gpio_num == PUSH_BUTTON_PIN1) {
		printf("Button1  pressed!\n");
	} else if (gpio_num == PUSH_BUTTON_PIN2) {
		printf("Button2  pressed!\n");
	}
    
  }
}

void app_main(void)
{
 
  gpio_set_direction(PUSH_BUTTON_PIN1, GPIO_MODE_INPUT);
  gpio_set_direction(PUSH_BUTTON_PIN2, GPIO_MODE_INPUT);
  
  gpio_set_direction(LED_PIN ,GPIO_MODE_OUTPUT);

  gpio_set_intr_type(PUSH_BUTTON_PIN1, GPIO_INTR_NEGEDGE);
  gpio_set_intr_type(PUSH_BUTTON_PIN2, GPIO_INTR_NEGEDGE);

  gpio_install_isr_service(ESP_INR_FLAG_DEFAULT);

  gpio_isr_handler_add(PUSH_BUTTON_PIN1, isr_handler, (void*)PUSH_BUTTON_PIN1); 
  gpio_isr_handler_add(PUSH_BUTTON_PIN2, isr_handler, (void*)PUSH_BUTTON_PIN2);
  

  xTaskCreate(interrupt_task, "interrupt_task", 4096, NULL, 10, &ISR);
}
