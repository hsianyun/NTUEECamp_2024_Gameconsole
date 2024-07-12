#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "joystick.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#define REPEAT_16_TIMES(x) x x x x x x x x x x x x x x x x
#define CALI_TIMES 16

void adc_handle()
{
    adc_oneshot_unit_handle_t hadc1;
    adc_oneshot_unit_init_cfg_t init_config1 = 
    {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &hadc1));

    adc_oneshot_chan_cfg_t ch_config1 = 
    {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(hadc1, ADC_CHANNEL_7, &ch_config1));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(hadc1, ADC_CHANNEL_6, &ch_config1));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(hadc1, ADC_CHANNEL_3, &ch_config1));

    int xoffset = 0, yoffset = 0, tmpx, tmpy;
    REPEAT_16_TIMES(adc_oneshot_read(hadc1, ADC_CHANNEL_7, &tmpx); xoffset += tmpx; adc_oneshot_read(hadc1, ADC_CHANNEL_6, &tmpy); yoffset += tmpy;
                    printf("tmpx: %d\ttmpy: %d\n", tmpx, tmpy);vTaskDelay(100 / portTICK_PERIOD_MS);)
    xoffset /= CALI_TIMES;
    yoffset /= CALI_TIMES;
    printf("xoffset: %d\tyoffset: %d\n", xoffset, yoffset);

    int x_raw = 0, y_raw = 0, x_val, y_val;
    while (1)
    {
        adc_oneshot_read(hadc1, ADC_CHANNEL_7, &x_raw);
        adc_oneshot_read(hadc1, ADC_CHANNEL_6, &y_raw);
        if(x_raw > xoffset)
            x_val = (x_raw-xoffset)*32767/(4095-xoffset);
        else
            x_val = (x_raw-xoffset)*32768/(xoffset);
        if(y_raw > yoffset)
            y_val = (y_raw-yoffset)*32767/(4095-yoffset);
        else
            y_val = (y_raw-yoffset)*32768/(yoffset);
        printf("x_raw-xoffset: %d\ty_raw-yoffset: %d\n", x_raw-xoffset, y_raw-yoffset);
        printf("x_val: %d\ty_val: %d\t angle: %.2f\n", x_val, y_val, atan2(y_val, x_val)*180/3.1415926);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    
    vTaskDelete(NULL);
}

void app_main(void)
{
    //xTaskCreate(adc_handle, "adc_handle", 2048, NULL, 5, NULL);
    Joystick* myJoystick = newJoystick();
    while(1)
    {
        detectJoystick(myJoystick);
        printf("x: %d\ty: %d\tangle: %.2f\tmagnitude: %.2f\n", getX(myJoystick), getY(myJoystick), getAngle(myJoystick), getMagnitude(myJoystick));
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    deleteJoystick(myJoystick);
}