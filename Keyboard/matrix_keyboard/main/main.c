#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>

// Define GPIO pins for the keypad rows and columns
const int ROW[2] = {GPIO_NUM_32, GPIO_NUM_33};      // Array for row pins
const int COLUMN[3] = {GPIO_NUM_14, GPIO_NUM_12, GPIO_NUM_13};  // Array for column pins

void app_main(void)
{
    // Initialize row pins as outputs and set them to HIGH
    for(int i = 0; i < 2; i++){
        gpio_set_direction(ROW[i], GPIO_MODE_OUTPUT);    // Set pin direction to OUTPUT
        gpio_set_level(ROW[i], 1);   // Set initial level to HIGH (active)
    }

    // Initialize column pins as inputs with pull-up resistors enabled
    for(int i = 0; i < 3; i++){
        gpio_set_direction(COLUMN[i], GPIO_MODE_INPUT);   // Set pin direction to INPUT
        gpio_set_pull_mode(COLUMN[i], GPIO_PULLUP_ONLY);  // Enable pull-up resistor
    }

    // Arrays to track the state of each key
    bool keyDown[6] = {};       // Array to track if keys are currently pressed
    bool keyWentDown[6] = {};   // Array to track if keys were just pressed

    // Main loop for scanning the keypad
    while (true) {
        // Iterate through each row
        for(int i = 0; i < 2; i++){
            gpio_set_level(ROW[i], 0);   // Activate the current row (set LOW)

            // Iterate through each column to check key status
            for(int j = 0; j < 3; j++){
                if(gpio_get_level(COLUMN[j]) == 0){    // Check if key is pressed (column is LOW)
                    if(keyDown[i*3+j] == 0){
                        keyWentDown[i*3+j] = 1;   // Key was just pressed
                    }
                    else{
                        keyWentDown[i*3+j] = 0;   // Key is still held down
                    }
                    keyDown[i*3+j] = 1;   // Mark key as pressed
                }
                else{
                    keyDown[i*3+j] = 0;   // Mark key as not pressed
                    keyWentDown[i*3+j] = 0; // Clear key-went-down status
                }
            }

            gpio_set_level(ROW[i], 1);   // Deactivate the current row (set HIGH)
        }

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