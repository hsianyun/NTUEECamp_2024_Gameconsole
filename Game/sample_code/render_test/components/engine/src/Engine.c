#include "Engine.h"
#include "Keyboard.h"
#include "Joystick.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

Engine* newEngine()
{
    Engine *engine = calloc(1, sizeof(Engine));
    engine->keyboard = newKeyboard();
    engine->joystick = newJoystick();
    engine->ResourceManager = newResourceManager();
    engine->RenderManager = newRenderManager();
    return engine;
}

void deleteEngine(Engine *engine)
{
    deleteKeyboard(engine->keyboard);
    deleteJoystick(engine->joystick);
    deleteResourceManager(engine->ResourceManager);
    deleteRenderManager(engine->RenderManager);
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


RenderResource* Engine_Render_findRenderResourceByID(Engine *engine, uint8_t ID)
{
    return renderManagerFindRenderResourceByID(engine->RenderManager, ID);
} 

RenderResource* Engine_Render_findRenderResourceByName(Engine *engine, char name[])
{
    return renderManagerFindRenderResourceByName(engine->RenderManager, name);
}

void Engine_Render_addImage(Engine *engine, char name[], uint16_t* image, uint16_t width, uint16_t height)
{
    renderManagerAddImage(engine->RenderManager, engine->ResourceManager, name, image, width, height);
}

void Engine_Render_addImageByFunction(Engine *engine, char name[], void (*function)(RenderResource*, uint16_t*), uint16_t width, uint16_t height)
{
    renderManagerAddImageByFunction(engine->RenderManager, engine->ResourceManager, name, function, width, height);
}

RenderObject* Engine_Render_newObject(Engine *engine, char name[], int16_t posX, int16_t posY, uint8_t visible)
{
    RenderObject* renderObject = calloc(1, sizeof(RenderObject));
    renderObjectNew(renderObject, Engine_Render_findRenderResourceByName(engine, name), posX, posY, visible);
    return renderObject;
}

void Engine_Render_deleteObject(Engine *engine, RenderObject* renderObject)
{
    if(renderObject != NULL) free(renderObject);
}

void Engine_Render_addObject(Engine *engine, RenderObject* renderObject)
{
    renderManagerAddObject(engine->RenderManager, renderObject);  
}

void Engine_Render_removeObject(Engine *engine, RenderObject* renderObject)
{
    renderManagerRemoveObject(engine->RenderManager, renderObject);  
}

void Engine_Render_render(Engine *engine, RenderObject* renderObject)
{
    renderManagerReRender(engine->RenderManager, renderObject);
}

void Engine_Render_clear(Engine *engine)
{
    renderManagerClear(engine->RenderManager);
}

void Engine_Render_update(Engine *engine)
{
    renderManagerUpdate(engine->RenderManager);
}