#include <stdio.h>
#include <math.h> 
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"

/* TODO 
1. pack into a component todo/ 
2. use adc_oneshot to get the value/ too hard
3. correct the value of center when init (should calculate the average value) 
7. the value of the angle in degree or radius (0~360 or 0~2pi)(type: float)
Thx
*/

int xPin = ADC1_CHANNEL_7;
int yPin = ADC1_CHANNEL_6;
int swPin = ADC1_CHANNEL_3;
  
int16_t xval=0;    
int16_t yval=0;    
int sw=0;  
int prev_sw=0;
float angle=0;

bool is_button_pressed = false;
bool is_button_released = false;

int16_t x_center = 0;
int16_t y_center = 0;

void setup_joystick() {
    printf("setup\n");
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(xPin, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(yPin, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(swPin, ADC_ATTEN_DB_12);
    for(int i = 0; i < 3; i++) {
        x_center += adc1_get_raw(xPin);
        y_center += adc1_get_raw(yPin);
    }
    x_center /= 3;
    y_center /= 3;
    printf("x_center: %d, y_center: %d\n", x_center, y_center);
}

inline int16_t get_joystick_x() {
    return adc1_get_raw(xPin);
}

inline int16_t get_joystick_y() {
    return adc1_get_raw(yPin);
}

inline int get_joystick_sw() {
    return adc1_get_raw(swPin);
}

void check_button_state() {
    if (prev_sw == 1 && sw == 0) {
        is_button_pressed = false;
        is_button_released = true;
    } else if (prev_sw == 0 && sw == 1) {
        is_button_pressed = true;
        is_button_released = false;
    } else {
        is_button_pressed = false;
        is_button_released = false;
    }
}

float calculate_angle() {
    // to be tested
    angle = atan2(yval-y_center, xval-x_center) * 180 / 3.14159265;
    angle = 180 - angle;
    if (angle < 0) {
        angle += 360;
    }
    return angle;
}

void app_main(void)
{
    setup_joystick();
    
    while (true)
    {
        prev_sw = sw;
        xval = get_joystick_x();
        yval = get_joystick_y();
        sw = get_joystick_sw();

        angle = calculate_angle();
        check_button_state();

        printf("VRx=%d, VRy=%d, SW=%d\n", xval, yval, sw);
        //printf("Hall Sensor: %d\n", RTCIO.hall_sens.hall_phase);
        printf("Angle: %f\n", angle);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    
}
