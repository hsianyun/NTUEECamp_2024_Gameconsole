#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "driver/timer.h"
#include "hal/timer_types.h"
#include "hal/timer_hal.h"

timer_config_t config = {
    .divider = 16,  // Clock divider
    .counter_dir = TIMER_COUNT_UP,  // Counting direction
    .counter_en = TIMER_PAUSE,      // Timer state
    .alarm_en = TIMER_ALARM_EN,     // Enable alarm
    .auto_reload = true,            // Auto-reload on alarm
};

void timerSetup(int i, int j)
{
    timer_init(i, j, &config);
    timer_set_counter_value(i, j, 0);
    timer_start(i, j);
    printf("Timer%d started\n", i*2+j+1);
}

void sendResponse()
{
    printf("Sending response to host...\n");
}

bool receiveInit()
{
    printf("Receiving init from host...\n");
    return true;
}

bool receiveTimeout()
{
    printf("Receiving timeout from host...\n");
    return true;
}

void clientStart()
{
    // dummy variable
    bool response  = true;
    if (!response) return;

    while(1) {
        if(receiveTimeout())  {
            printf("Received timeout from host...\n");
            return;
        } else {
            if(receiveInit()) {
                printf("Receiving init from host...\n");
                timerSetup(1, 0);
                // send ack
                // enter game
                // timer3start
            }
        }
    }
}
