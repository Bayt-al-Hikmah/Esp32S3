#include "i2c_slave.h"

i2c_slave_context_t context = {0};

static bool i2c_slave_receive_cb(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_rx_done_event_data_t *evt_data, void *arg){

    context.data[0] = evt_data->buffer[0];
    context.data[1] = evt_data->buffer[1];
    context.data[2] = evt_data->buffer[2];
    return 0;
}

void i2c_slave_init(){

    i2c_slave_config_t i2c_slv_config = {        
    .i2c_port = I2C_PORT,        
    .clk_source = I2C_CLK_SRC_DEFAULT,        
    .scl_io_num = I2C_SLAVE_SCL_IO,        
    .sda_io_num = I2C_SLAVE_SDA_IO,        
    .slave_addr = SLAVE_ADDR,        
    .send_buf_depth = 100,        
    .receive_buf_depth = 100,        
    .flags.enable_internal_pullup = true,    
    };

    i2c_slave_dev_handle_t slave_handle;    
    i2c_new_slave_device(&i2c_slv_config, &slave_handle);

    i2c_slave_event_callbacks_t cbs = {
        .on_receive = i2c_slave_receive_cb,
    };
    i2c_slave_register_event_callbacks(slave_handle, &cbs, &context);
}