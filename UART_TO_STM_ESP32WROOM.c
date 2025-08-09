#include <stdio.h>
#include <string.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void){

	uart_config_t uartconfig = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.source_clk = UART_SCLK_APB,
	};

	uart_param_config(UART_NUM_2,&uartconfig);
	uart_set_pin(UART_NUM_2, GPIO_NUM_17, GPIO_NUM_16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	uart_driver_install(UART_NUM_2, 1024,0, 0, NULL, 0);

	char* message = "FWD100\r\n";

	while(1){
		uart_write_bytes(UART_NUM_2, message, strlen(message));
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}

}
