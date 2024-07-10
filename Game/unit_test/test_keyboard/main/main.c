#include <stdio.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "engine.h"

void handleKeyboard(void* pvParameter);

void app_main(void)
{
    xTaskCreate(handleKeyboard, "handleKeyboard", 2048, NULL, 0, NULL);
}

void handleKeyboard(void* pvParameter)
{
    Engine *engine = newEngine();
    while(1)
    {
        Engine_Detect(engine);
        for(uint8_t key = 0; key<64; key++)
        {
            printf("%#x: %d", key, Engine_Keyboard_isKeyRelease(engine, key));
            if(key % 8 == 7)
            {
                printf("\n");
            }
            else
            {
                printf("\t");
            }
        }
        printf("\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
