#include <stdio.h>
#include "buttons.h"


void app_main(void){
    int32_t counter = 0;
    init_nvs(&counter);
    xTaskCreatePinnedToCore(count_up, "Count up task", 2048, &counter, 1, NULL, 1);
    xTaskCreatePinnedToCore(reset, "Reset task", 2048, &counter, 1, NULL, 1);

}
