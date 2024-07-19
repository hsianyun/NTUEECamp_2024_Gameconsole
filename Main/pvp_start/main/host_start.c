#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "driver/timer.h"
#include "hal/timer_types.h"
#include "hal/timer_hal.h"
#include "client_start.c"
#include "uartFunction.h"

double timer_val_1 = 0;
double timer_val_2 = 0;

void startGameAsHost()
{
    printf("starting game as host...\n");
}


void hostStart()
{
    sendRequest();

    // wait for client to accept request
    while(1) {
        bool response = acceptRequest();
        if (response) {
            printf("Client responded to request\n");
            break;
        }
    }

    // game loop to select character
    pickCharacter();

    // wait for client to select character
    while(1)    {
        bool response = acceptRequest();
        if (response) {
            printf("Client has chosen his character\n");
            break;
        }
    }


    startGameAsHost();
}
