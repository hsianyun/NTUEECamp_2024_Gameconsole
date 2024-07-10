#ifndef _ENGINE_H
#define _ENGINE_H

#include "keyboard.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct engine
{
    Keyboard *keyboard;
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



//engine.h
#endif