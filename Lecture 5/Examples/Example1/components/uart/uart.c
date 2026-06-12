#include "uart.h"
  
void uart_init(){
   uart_config_t uart_config = {  
    .baud_rate = 115200,  
	.data_bits = UART_DATA_8_BITS,  
	.parity = UART_PARITY_DISABLE,  
	.stop_bits = UART_STOP_BITS_1,  
	.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,  
	.source_clk = UART_SCLK_DEFAULT,  
	};

    uart_driver_install(UART_PORT, BUF_SIZE, 0, 0, NULL, 0);  
    uart_param_config(UART_PORT, &uart_config);  
    uart_set_pin(UART_PORT, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);  

}


