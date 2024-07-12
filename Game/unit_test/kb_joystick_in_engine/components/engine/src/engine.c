#include "engine.h"
#include "keyboard.h"
#include "joystick.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

Engine* newEngine()
{
    Engine *engine = calloc(1, sizeof(Engine));
    engine->keyboard = newKeyboard();
    engine->joystick = newJoystick();
    return engine;
}

void deleteEngine(Engine *engine)
{
    deleteKeyboard(engine->keyboard);
    deleteJoystick(engine->joystick);
    free(engine);
}

void Engine_Detect(Engine *engine)
{
    Keyboard_Detect(engine->keyboard);
    Joystick_Detect(engine->joystick);
}

uint8_t Engine_Keyboard_getKeyDown(Engine *engine)
{
    return Keyboard_getKeyDown(engine->keyboard);
}

uint8_t Engine_Keyboard_getKeyPress(Engine *engine)
{
    return Keyboard_getKeyPress(engine->keyboard);
}

uint8_t Engine_Keyboard_getKeyRelease(Engine *engine)
{
    return Keyboard_getKeyRelease(engine->keyboard);
}

bool Engine_Keyboard_isKeyDown(Engine *engine, uint8_t key)
{
    return Keyboard_isKeyDown(engine->keyboard, key);
}

bool Engine_Keyboard_isKeyPress(Engine *engine, uint8_t key)
{
    return Keyboard_isKeyPress(engine->keyboard, key);
}

bool Engine_Keyboard_isKeyRelease(Engine *engine, uint8_t key)
{
    return Keyboard_isKeyRelease(engine->keyboard, key);
}

bool Engine_Joystick_notZero(Engine *engine)
{
    return Joystick_notZero(engine->joystick);
}

int16_t Engine_Joystick_getX(Engine *engine)
{
    return Joystick_getX(engine->joystick);
}

int16_t Engine_Joystick_getY(Engine *engine)
{
    return Joystick_getY(engine->joystick);
}

bool Engine_Joystick_getButton(Engine *engine)
{
    return Joystick_getButton(engine->joystick);
}

double Engine_Joystick_getAngle(Engine *engine)
{
    return Joystick_getAngle(engine->joystick);
}

double Engine_Joystick_getMagnitude(Engine *engine)
{
    return Joystick_getMagnitude(engine->joystick);
}

void Engine_Joystick_setThreshold(Engine *engine, uint8_t threshold)
{
    Joystick_setThreshold(engine->joystick, threshold);
}