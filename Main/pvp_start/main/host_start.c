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
#include "client_start.c"

uint16_t timer_val_1 = 0;
uint16_t timer_val_2 = 0;
int response = 0;       // 0: no response, 1: response no, 2: response yes
bool reveice_ack = false;

void getResponse(int* response)
{
    printf("Getting response from client...\n");
}

void sendTimeout()
{
    printf("Sending timeout to client...\n");
}

void sendInit()
{
    printf("Sending init to client...\n");
}

bool receiveAck()
{
    printf("Receiving ack from client...\n");
    return true;
}

void hostStart()
{
    timerSetup(0, 0);
    bool start = false;

    while(!start)    {
        timer_get_counter_value(0, 0, &timer_val_1);
        if(timer_val_1 >= 10000)  {
            printf("No response from client during request\n");
            return;
        } else {
            getResponse(&response);
            switch (response)   {
                case 0:
                    printf("Waiting for response...\n");
                    break;
                case 1:
                    printf("Client declined the request\n");
                    return;
                case 2:
                    printf("Client accepted the request\n");
                    start = true;
                    timer_pause(0, 0);
                    break;
            }
        }
    }

    timerSetup(0, 1);
    sendInit();

    while(1)    {
        timer_get_counter_value(0, 1, &timer_val_2);
        if(timer_val_2 >= 10000)  {
            printf("No response from client during init\n");
            sendTimeout();
            return;
        } else {
            reveice_ack = receiveAck();
            if(reveice_ack) {
                printf("Client received the init\n");
                timerSetup(1, 0);
                break;
            }
        }
    }
}
