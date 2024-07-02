#include <driver/gpio.h>
#include "keyboardlib.h"

const int ROW[2] = {GPIO_NUM_32, GPIO_NUM_33};      // Array for row pins
const int COLUMN[3] = {GPIO_NUM_14, GPIO_NUM_12, GPIO_NUM_13};  // Array for column pins

void keyboardSetup(){
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
}

void detectKeyboard(bool keyDown[], bool keyWentDown[]){
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
}