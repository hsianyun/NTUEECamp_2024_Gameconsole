#include "engine.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

Engine* newEngine()
{
    Engine *engine = calloc(1, sizeof(Engine));
    engine->keyboard = newKeyboard();
    return engine;
}

void deleteEngine(Engine *engine)
{
    deleteKeyboard(engine->keyboard);
    free(engine);
}

void Engine_Detect(Engine *engine)
{
    Keyboard_Detect(engine->keyboard);
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

