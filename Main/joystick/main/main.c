#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "driver/adc.h"


int xPin = ADC1_CHANNEL_4;  
int yPin = ADC1_CHANNEL_7;  
int zPin = ADC1_CHANNEL_6;
  
int xval=0;    //Ｘ軸變數
int yval=0;    //Ｙ軸變數
int zval=0;    //Ｚ軸變數


void setup() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(xPin, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(yPin, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(zPin, ADC_ATTEN_DB_11);
}

void loop() {
    xval = adc1_get_raw(xPin);  
    yval = adc1_get_raw(yPin);  
    zval = adc1_get_raw(zPin);  


  char buf[100];
  sprintf(buf, "VRx=%d, VRy=%d, SW=%d", xval, yval, zval);
  printf("%s\n", buf);

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
