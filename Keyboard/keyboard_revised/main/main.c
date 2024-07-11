#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include "keyboardlib.h"

void handleKeyboard(void);

void app_main(void)
{
    keyboardSetup();
    xTaskCreate(handleKeyboard, "handleKeyboard", 2048, NULL, 5, NULL);
}

void handleKeyboard(void)
{
    while(1)
    {
        detectKeyboard();
        printKeyPress();
        printKeyState();
        printKeyRelease();
        int u = getUltimateSkill();
        if(u != -1){
            printf("Ult: %d\n", u);
            // printf("********************************\n");
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
