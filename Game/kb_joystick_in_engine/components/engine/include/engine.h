#ifndef _ENGINE_H
#define _ENGINE_H

#include "keyboard.h"
#include "joystick.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct engine
{
    Keyboard *keyboard;
    Joystick *joystick;
} Engine;

Engine* newEngine();
void deleteEngine(Engine *engine);
void Engine_Detect(Engine *engine);

uint8_t Engine_Keyboard_getKeyDown(Engine *engine);
uint8_t Engine_Keyboard_getKeyPress(Engine *engine);
uint8_t Engine_Keyboard_getKeyRelease(Engine *engine);
bool Engine_Keyboard_isKeyDown(Engine *engine, uint8_t key);
bool Engine_Keyboard_isKeyPress(Engine *engine, uint8_t key);
bool Engine_Keyboard_isKeyRelease(Engine *engine, uint8_t key);

bool Engine_Joystick_notZero(Engine *engine);
int16_t Engine_Joystick_getX(Engine *engine);
int16_t Engine_Joystick_getY(Engine *engine);
bool Engine_Joystick_getButton(Engine *engine);
double Engine_Joystick_getAngle(Engine *engine);
double Engine_Joystick_getMagnitude(Engine *engine);
void Engine_Joystick_setThreshold(Engine *engine, uint8_t threshold);

//engine.h
#endif