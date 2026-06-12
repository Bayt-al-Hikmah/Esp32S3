#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_slave.h"
#include "driver/gpio.h"

#define I2C_SLAVE_SCL_IO 9
#define I2C_SLAVE_SDA_IO 8
#define I2C_PORT I2C_NUM_0
#define SLAVE_ADDR 0x28

typedef struct {
    uint8_t data[10];
} i2c_slave_context_t;

extern i2c_slave_context_t context;

void i2c_slave_init();

#endif