#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include "keyboardlib.h"

void app_main(void)
{
    keyboardSetup();

    // Arrays to track the state of each key
    bool keyDown[6] = {};       // Array to track if keys are currently pressed
    bool keyWentDown[6] = {};   // Array to track if keys were just pressed

    // Main loop for scanning the keypad
    while (true) {
        
        detectKeyboard(keyDown, keyWentDown);

        // Print the state of keys
        printf("keyDown: ");
        for(int i = 0; i < 6; i++){
            if(keyDown[i]) printf("%d ", i);   // Print index of currently pressed keys
        }
        printf("\nkeyWentDown: ");
        for(int i = 0; i < 6; i++){
            if(keyWentDown[i]) printf("%d ", i);   // Print index of keys that were just pressed
        }
        printf("\n");

        vTaskDelay(100 / portTICK_PERIOD_MS);   // Delay to prevent continuous polling
    }
}