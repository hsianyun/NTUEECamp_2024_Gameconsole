#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "host_start.c"

char* role = "undefined";
bool find = false;
bool searchRequest() {
    printf("Searching PVP request...\n");
    return true;
}

bool startButtonClicked() {
    printf("Checking if start button is clicked...\n");
    return true;
}

void sendRequest() {
    printf("Sending PVP request...\n");
}

void app_main(void)
{
    while(!find)    {
        find = searchRequest();
        if(startButtonClicked())    {
            sendRequest();
            find = true;
            role = "host";
        }
    }
    if(find)    {
        printf("PVP request found!\n");
        role = "client";
        clientStart();
    } else {
        printf("No PVP request found!\n");
        hostStart();
    }
}
