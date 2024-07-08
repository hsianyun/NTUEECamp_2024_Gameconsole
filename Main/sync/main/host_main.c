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

int time_out_frame = 0;
bool receive_ack = false;
bool receive_delay = false;
uint16_t timer_val = 0;
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
    printf("Timer%d started\n", i*2+j+1);
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

void sendTimer(int i) {
    printf("Sending timer%d...\n", i);
}

bool receiveAck() {
    printf("Receiving ack...\n");
    return true;
}

bool receiveDelay() {
    printf("Receiving delay...\n");
    return false;
}

void enter_Load_Page() {
    printf("Entering load page...\n");
}

void computeAndRender() {
    printf("Computing and rendering...\n");
}

void app_main(void)
{
    readInput();
    sendObject();
    receiveObject();
Retry:
    sendTimer(3);
    timerSetup(1, 1);
    while(1)    {
        receive_ack = receiveAck();
        receive_delay = receiveDelay();
        timer_get_counter_value(1, 1, &timer_val);
        if (receive_ack) {
            break;
        } else if(receive_delay)    {
            vTaskDelay(50 / portTICK_PERIOD_MS);
            break;
        } else if(timer_val > 100) {
            timer_pause(1, 1);
            time_out_frame++;
            if (time_out_frame > 20) {
                enter_Load_Page();
                goto Retry;
            } else {
                break;
            }
        }
    }
    computeAndRender();
}
