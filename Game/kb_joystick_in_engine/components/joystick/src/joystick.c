#include "joystick.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

#define REPEAT_8_TIMES(x) x x x x x x x x
#define CALI_INTERVAL 50

Joystick* newJoystick()
{
    Joystick* joystick = malloc(sizeof(Joystick));
    joystick->button = false;
    joystick->x_offset = 2048;
    joystick->y_offset = 2048;
    joystick->threshold = 4;

    adc_oneshot_unit_init_cfg_t init_config1 = 
    {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    if(adc_oneshot_new_unit(&init_config1, &(joystick->hadc1)) != ESP_OK)
    {
        ESP_LOGE("JOYSTICK", "Failed to initialize ADC unit");
        free(joystick);
        return NULL;
    }

    adc_oneshot_chan_cfg_t ch_config1 = 
    {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    if(adc_oneshot_config_channel(joystick->hadc1, ADC_CHANNEL_7, &ch_config1) || adc_oneshot_config_channel(joystick->hadc1, ADC_CHANNEL_6, &ch_config1))
    {
        ESP_LOGE("JOYSTICK", "Failed to configure ADC channel");
        free(joystick);
        return NULL;
    }

    int x_sum = 0, y_sum = 0, x_tmp, y_tmp;
    REPEAT_8_TIMES(adc_oneshot_read(joystick->hadc1, ADC_CHANNEL_7, &x_tmp); x_sum += x_tmp; 
                    adc_oneshot_read(joystick->hadc1, ADC_CHANNEL_6, &y_tmp); y_sum += y_tmp; 
                    vTaskDelay(CALI_INTERVAL / portTICK_PERIOD_MS);)
    joystick->x_offset = x_sum / 8;
    joystick->y_offset = y_sum / 8;
    joystick->y_offset = 4096 - joystick->y_offset;



    return joystick;
}

void deleteJoystick(Joystick* joystick)
{
    free(joystick);
}

void detectJoystick(Joystick* joystick)
{
    int x_raw, y_raw;
    adc_oneshot_read(joystick->hadc1, ADC_CHANNEL_7, &x_raw);
    adc_oneshot_read(joystick->hadc1, ADC_CHANNEL_6, &y_raw);
    y_raw = 4095 - y_raw;
    if(abs(x_raw - joystick->x_offset) > joystick->threshold)
    {
        if(x_raw > joystick->x_offset)
            joystick->x = (x_raw - joystick->x_offset) * 32767 / (4095 - joystick->x_offset);
        else
            joystick->x = (x_raw - joystick->x_offset) * 32768 / joystick->x_offset;
    }
    else
        joystick->x = 0;
    
    if(abs(y_raw - joystick->y_offset) > joystick->threshold)
    {
        if(y_raw > joystick->y_offset)
            joystick->y = (y_raw - joystick->y_offset) * 32767 / (4095 - joystick->y_offset);
        else
            joystick->y = (y_raw - joystick->y_offset) * 32768 / joystick->y_offset;
    }
    else
        joystick->y = 0;
}

bool notZero(Joystick* joystick)
{
    int x_raw, y_raw;
    adc_oneshot_read(joystick->hadc1, ADC_CHANNEL_7, &x_raw);
    adc_oneshot_read(joystick->hadc1, ADC_CHANNEL_6, &y_raw);
    return abs(x_raw - joystick->x_offset) > joystick->threshold || abs(y_raw - joystick->y_offset) > joystick->threshold;
}

int16_t getX(Joystick* joystick)
{
    return joystick->x;
}

int16_t getY(Joystick* joystick)
{
    return joystick->y;
}

bool getButton(Joystick* joystick)
{
    return joystick->button;
}

double getAngle(Joystick* joystick)
{
    return atan2(joystick->y, joystick->x) * 180 / 3.1415926;
}

double getMagnitude(Joystick* joystick)
{
    return sqrt((double)joystick->x * (double)joystick->x + (double)joystick->y * (double)joystick->y);
}

void setThreshold(Joystick* joystick, uint8_t threshold)
{
    joystick->threshold = threshold;
}