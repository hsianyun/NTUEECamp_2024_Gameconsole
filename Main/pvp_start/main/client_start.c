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
#include "uartFunction.h"



void pickCharacter()
{
    printf("Picking character...\n");
}

void startGameAsClient()
{
    printf("Starting game as client...\n");
}

// run this when client accepts request
void clientStart()
{
    // tells host you have accepted request
    sendRequest();
    // game loop to pick character
    pickCharacter();
    // tell host you are ready
    sendRequest();
    
    startGameAsClient();
}
