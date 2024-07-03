#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "driver/adc.h"
#include <driver/gpio.h>

// define as global variables
int xPin = ADC1_CHANNEL_7;  
int yPin = ADC1_CHANNEL_6;  

int x_val = 0;
int y_val = 0;
int z_val = 0;
int previous_z_val = 0;

// setup
void setup_joystick() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(xPin, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(yPin, ADC_ATTEN_DB_12);
    gpio_set_direction(GPIO_NUM_39, GPIO_MODE_INPUT);
}

// get x value
inline int get_joystick_x() {
    return adc1_get_raw(xPin);  
}

// get y value
inline int get_joystick_y() {
    return adc1_get_raw(yPin);  
}

// get z value
inline int get_joystick_z() {
    return gpio_get_level(GPIO_NUM_39);
}

inline void set_previous_z_val() {
    previous_z_val = z_val;
}

void app_main(void)
{
    setup_joystick();
    
    while (true)
    {
        set_previous_z_val();
        x_val = get_joystick_x();
        y_val = get_joystick_y();
        z_val = get_joystick_z();
    }
    
}
