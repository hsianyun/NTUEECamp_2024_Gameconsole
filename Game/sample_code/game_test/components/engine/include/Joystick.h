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
void Joystick_Detect(Joystick* joystick);
bool Joystick_notZero(Joystick* joystick);
int16_t Joystick_getX(Joystick* joystick);
int16_t Joystick_getY(Joystick* joystick);
bool Joystick_getButton(Joystick* joystick);
double Joystick_getAngle(Joystick* joystick);
double Joystick_getMagnitude(Joystick* joystick);
void Joystick_setThreshold(Joystick* joystick, uint8_t threshold);



// joystick.h
#endif