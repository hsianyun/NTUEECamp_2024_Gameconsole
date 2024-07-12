#include <driver/gpio.h>
#include "keyboardlib.h"

#define GPIO_HIGH 1
#define GPIO_LOW 0

const uint8_t ROW[NUM_OF_ROW] = {GPIO_NUM_33, GPIO_NUM_32};      // Array for row pins
const uint8_t COLUMN[NUM_OF_COL] = {GPIO_NUM_14, GPIO_NUM_12, GPIO_NUM_13};  // Array for column pins

uint8_t last_key = 0b00000000, curr_key = 0b00000000;    // Variables to store the last and current key states
uint8_t key_press = 0b00000000, key_release = 0b00000000;    // Variables to store the key press and release states

void keyboardInit(void){
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
    // Swap the current and last key state
    last_key = curr_key;
    curr_key = 0;

    
    gpio_set_level(ROW[0], GPIO_LOW);   // Activate the current row (set LOW)

    curr_key += (!gpio_get_level(COLUMN[0]) << (0));   // Set the key state in the current frame
    curr_key += (!gpio_get_level(COLUMN[1]) << (1));
    curr_key += (!gpio_get_level(COLUMN[2]) << (2));

    gpio_set_level(ROW[0], GPIO_HIGH);   // Deactivate the current row (set HIGH)
    
    gpio_set_level(ROW[1], GPIO_LOW);   // Activate the current row (set LOW)

    curr_key += (!gpio_get_level(COLUMN[0]) << (3));   // Set the key state in the current frame
    curr_key += (!gpio_get_level(COLUMN[1]) << (4));
    curr_key += (!gpio_get_level(COLUMN[2]) << (5));

    gpio_set_level(ROW[1], GPIO_HIGH);   // Deactivate the current row (set HIGH)

    key_press = curr_key & ~last_key;    // Calculate the key press state
    key_release = ~curr_key & last_key;  // Calculate the key release state
}

const uint8_t* registerKeyPress(void){
    return &key_press;
}

const uint8_t* registerKeyState(void){
    return &curr_key;
}

const uint8_t* registerKeyRelease(void){
    return &key_release;
}



// Test
/*
void printKeyState(void){
    printf("Key state:\n");
    printf("%#x\n", curr_key);
}

void printKeyPress(void){
    printf("Key press:\n");
    printf("%#x\n", curr_key & ~last_key);
}

void printKeyRelease(void)
{
    printf("Key release:\n");
    printf("%#x\n", ~curr_key & last_key);
}
*/