#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "driver/adc.h"
//#include "esp_adc/adc_oneshot.h"

/* TODO 
1. pack into a component
2. use adc_oneshot to get the value 
3. correct the value of center when init (should calculate the average value)
4. x, y value getter in int16_t(signed) which is the corrected value
5. the value of the angle in degree or radius (0~360 or 0~2pi)(type: float)
Thx
*/

int xPin = ADC1_CHANNEL_7;
int yPin = ADC1_CHANNEL_6;
int swPin = GPIO_NUM_39;
  
uint16_t xval=0;    //Ｘ軸變數
uint16_t yval=0;    //Ｙ軸變數
int sw=0;    //Ｚ軸變數


void setup() {
    printf("setup\n");
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(xPin, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(yPin, ADC_ATTEN_DB_12);
    gpio_set_direction(swPin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(swPin, GPIO_PULLUP_ONLY);
}

void loop() {
    xval = adc1_get_raw(xPin);
    yval = adc1_get_raw(yPin);
    sw = !gpio_get_level(swPin);


  printf("VRx=%d, VRy=%d, SW=%d\n", xval, yval, sw);
  //printf("Hall Sensor: %d\n", RTCIO.hall_sens.hall_phase);

  vTaskDelay(100 / portTICK_PERIOD_MS);
}

void app_main(void)
{
    setup();
    
    while (true)
    {
        loop();
    }
    
}
