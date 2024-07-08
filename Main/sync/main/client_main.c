#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "hal/timer_hal.h"
#include "hal/timer_types.h"
#include "driver/timer.h"

uint16_t timer_val_3 = 0;
uint16_t host_timer_val_3 = 0;
timer_config_t config = {
    .divider = 16,                  // Clock divider
    .counter_dir = TIMER_COUNT_UP,  // Counting direction
    .counter_en = TIMER_PAUSE,      // Timer state
    .alarm_en = TIMER_ALARM_EN,     // Enable alarm
    .auto_reload = true,            // Auto-reload on alarm
};

void timerSetup(int i, int j) {
    timer_init(i, j, &config);
    timer_set_counter_value(i, j, 0);
    timer_start(i, j);
    printf("Timer%d setup finished\n", i*2+j+1);
}

void readInput() {
    printf("Reading input...\n");
}

void sendObject() {
    printf("Sending object...\n");
}

void receiveObject() {
    printf("Receiving object...\n");
}

uint16_t getHostTimer(int i, int j) {
    printf("Getting host timer%d...\n", i*2+j+1);
    return 0;
}

void sendDelay(int delay) {
    printf("Sending delay %dseconds...\n", delay);
}

void computeAndRender() {
    printf("Computing and rendering...\n");
}

void app_main(void)
{
    readInput();
    sendObject();
    receiveObject();
    host_timer_val_3 = getHostTimer(1, 0);
    timer_get_counter_value(1, 0, &timer_val_3);
    int error = host_timer_val_3 - timer_val_3;
    if (error > 0) {
        printf("host's timer is %d faster\n", error);
        sendDelay(error);
    } else if(error < 0) {
        printf("host's timer is %d slower\n", -error);
        vTaskDelay(-error / portTICK_PERIOD_MS);
    } else {
        printf("host's timer is in sync\n");
    }

    computeAndRender();
}
