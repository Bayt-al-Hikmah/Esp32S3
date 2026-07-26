#include "moisture_sensor.h"




void moisture_sensor_task(void *pvParameters){
    int moisture_data;
    char message[] = "event: Moisture Sensor\r\n"
            "data: 0000\r\n"
            "\r\n";

    while(1){
    adc_oneshot_read(adc1_handle, ADC_CHANNEL_1, &moisture_data);
    int i = 4;
    while(moisture_data > 0){
        message[37 - i] = (moisture_data % 10) + '0';
        moisture_data /= 10;
        i++;
    }
    send_sse_message(message);
    vTaskDelay(pdMS_TO_TICKS(1000));  
    }
}