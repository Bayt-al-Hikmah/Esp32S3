#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"

#include "driver/gpio.h"
#include <stdbool.h>

#define I2C_MASTER_SCL_IO 9
#define I2C_MASTER_SDA_IO 8
#define I2C_PORT I2C_NUM_0
#define SLAVE_ADDR 0x28

  
void i2c_master_init();
    
extern i2c_master_dev_handle_t dev_handle; 

#endif 