#ifndef _JOYSTICK_H
#define _JOYSTICK_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_adc/adc_oneshot.h"

typedef struct joystick
{
    adc_oneshot_unit_handle_t hadc1;
    int16_t x;
    int16_t y;
    bool button;
    int16_t x_offset;
    int16_t y_offset;
    uint8_t threshold;
} Joystick;

Joystick* newJoystick();
void deleteJoystick(Joystick* joystick);
void detectJoystick(Joystick* joystick);
bool notZero(Joystick* joystick);
int16_t getX(Joystick* joystick);
int16_t getY(Joystick* joystick);
bool getButton(Joystick* joystick);
double getAngle(Joystick* joystick);
double getMagnitude(Joystick* joystick);
void setThreshold(Joystick* joystick, uint8_t threshold);



// joystick.h
#endif