#include "light_sensor.h"



void light_sensor_task(void *pvParameters){
    int light_data;
    char message[] = "event: Light Sensor\r\n"
            "data: 0000\r\n"
            "\r\n";
    while(1){
    adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &light_data);
    int i = 4;
     while(light_data > 0){
        
        message[34 - i] = (light_data % 10) + '0';
        light_data /= 10;
        i++;
    }
    send_sse_message(message);
    vTaskDelay(pdMS_TO_TICKS(1000));  
    }
}
