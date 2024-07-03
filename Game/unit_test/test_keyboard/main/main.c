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
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
