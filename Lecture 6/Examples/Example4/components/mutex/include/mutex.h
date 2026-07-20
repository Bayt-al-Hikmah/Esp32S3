#ifndef MUTEXX_H
#define MUTEXX_H


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <stdio.h>

extern SemaphoreHandle_t terminal_mutex;

#endif
