/*
    Simple Game Engine for ESP32
*/

/*
    Engine.h

    Main include header for the whole engine library.
*/

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "Resource.h"
#include "Render.h"
#include "Keyboard.h"
#include "Joystick.h"

typedef struct engine
{
    Keyboard *keyboard;
    Joystick *joystick;
    ResourceManager *ResourceManager;
    RenderManager *RenderManager;
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

RenderResource* Engine_Render_findRenderResourceByID(Engine *engine, uint8_t ID); 
RenderResource* Engine_Render_findRenderResourceByName(Engine *engine, char name[]); 
void Engine_Render_addImage(Engine *engine, char name[], uint16_t* image, uint16_t width, uint16_t height);
RenderObject* Engine_Render_newObject(Engine *engine, char name[], int16_t posX, int16_t posY, uint8_t visible);
void Engine_Render_deleteObject(Engine *engine, RenderObject* renderObject);
void Engine_Render_addObject(Engine *engine, RenderObject* renderObject);   
void Engine_Render_removeObject(Engine *engine, RenderObject* renderObject); 
void Engine_Render_render(Engine *engine, RenderObject* renderObject);
void Engine_Render_clear(Engine *engine);
void Engine_Render_update(Engine *engine);

#endif /* _ENGINE_H_ */