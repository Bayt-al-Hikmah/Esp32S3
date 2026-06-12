#ifndef UART_H
#define UART_H

#include "freertos/FreeRTOS.h"  
#include "freertos/task.h"  
#include "driver/uart.h"  
#include "driver/gpio.h"
#include <stdbool.h>

#define UART_PORT UART_NUM_1  
#define UART_TX_PIN 17  
#define UART_RX_PIN 18  
#define BUF_SIZE 1024  

void uart_init();

#endif