#include <driver/gpio.h>
#include "keyboardlib.h"

#define GPIO_HIGH 1
#define GPIO_LOW 0

const uint8_t ROW[NUM_OF_ROW] = {GPIO_NUM_32, GPIO_NUM_33};      // Array for row pins
const uint8_t COLUMN[NUM_OF_COL] = {GPIO_NUM_14, GPIO_NUM_12, GPIO_NUM_13};  // Array for column pins

bool mem1[NUM_OF_ROW*NUM_OF_COL] = {0}, mem2[NUM_OF_ROW*NUM_OF_COL] = {0};
bool *curr_key_state = mem1, *last_key_state = mem2; // Array to store key states in current and last frame. 0 = not pressed, 1 = pressed

void keyboardSetup(void){
    // Initialize row pins as outputs and set them to HIGH
    for(int i = 0; i < NUM_OF_ROW; i++){
        gpio_set_direction(ROW[i], GPIO_MODE_OUTPUT);    // Set pin direction to OUTPUT
        gpio_set_level(ROW[i], 1);   // Set initial level to HIGH (active)
    }

    // Initialize column pins as inputs with pull-up resistors enabled
    for(int i = 0; i < NUM_OF_COL; i++){
        gpio_set_direction(COLUMN[i], GPIO_MODE_INPUT);   // Set pin direction to INPUT
        gpio_set_pull_mode(COLUMN[i], GPIO_PULLUP_ONLY);  // Enable pull-up resistor
    }
}

// Should be called by the main loop to detect key presses in each frame
void detectKeyboard(void){

    // Swap the current and last key state arrays
    bool *tmp = last_key_state;
    last_key_state = curr_key_state;
    curr_key_state = tmp;

    for(int i = 0; i < NUM_OF_ROW; i++){
        gpio_set_level(ROW[i], GPIO_LOW);   // Activate the current row (set LOW)

        // Iterate through each column to check key status
        for(int j = 0; j < NUM_OF_COL; j++){
            curr_key_state[i*NUM_OF_COL + j] = !gpio_get_level(COLUMN[j]);  // Set the key state in the current frame
        }

        gpio_set_level(ROW[i], GPIO_HIGH);   // Deactivate the current row (set HIGH)
    }
}

// Test
void printKeyState(void){
    printf("Key state:\n");
    for(int i = 0; i < NUM_OF_ROW; i++){
        for(int j = 0; j < NUM_OF_COL; j++){
            printf("%d ", curr_key_state[i*NUM_OF_COL + j]);
        }
        printf("\n");
    }
    //printf("\n");
}

void printKeyPress(void){
    printf("Key press:\n");
    for(int i = 0; i < NUM_OF_ROW; i++){
        for(int j = 0; j < NUM_OF_COL; j++){
            printf("%d ", curr_key_state[i*NUM_OF_COL + j] > last_key_state[i*NUM_OF_COL + j]);
        }
        printf("\n");
    }
    //printf("\n");
}

void printKeyRelease(void)
{
    printf("Key release:\n");
    for(int i = 0; i < NUM_OF_ROW; i++){
        for(int j = 0; j < NUM_OF_COL; j++){
            printf("%d ", curr_key_state[i*NUM_OF_COL + j] < last_key_state[i*NUM_OF_COL + j]);
        }
        printf("\n");
    }
    //printf("\n");
}