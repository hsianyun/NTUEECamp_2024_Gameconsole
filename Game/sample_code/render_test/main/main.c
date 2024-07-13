#include <stdio.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiffs.h"

#include "st7789.h"
#include "fontx.h"
#include "pngle.h"
#include "decode_png.h"

#include "Engine.h"

#include "../files/people.h"

//--class Person--//
typedef struct person
{
    RenderObject *mRenderObject;
    int attack;
    int HP;
    int oriX;
    int oriY;

    void (*move)(struct person*);
}Person;

void personMove(Person* obj)
{
    if(obj->mRenderObject->mPosX + (5 * obj->oriX) <= 0 || obj->mRenderObject->mPosX + (5 * obj->oriX) + 50 >= 320)
    {
        obj->oriX *= -1;
        obj->mRenderObject->setVisible(obj->mRenderObject, -1 * obj->mRenderObject->mVisible);
    }
    if(obj->mRenderObject->mPosY + (5 * obj->oriY) <= 0 || obj->mRenderObject->mPosY + (5 * obj->oriY) + 50 >= 240)
    {
        obj->oriY *= -1;
        obj->mRenderObject->setVisible(obj->mRenderObject, -1 * obj->mRenderObject->mVisible);
    }
    obj->mRenderObject->setPos(obj->mRenderObject, obj->mRenderObject->mPosX + (5 * obj->oriX), obj->mRenderObject->mPosY + (5 * obj->oriY));
}

void newPerson(Person* obj, Engine* engine, int16_t posX, int16_t posY)
{
    obj->move = personMove;
    obj->oriX = 1;
    obj->oriY = 1;
    obj->attack = 10;
    obj->HP = 100;
    obj->mRenderObject = Engine_Render_newObject(engine, "person", posX, posY, 1);
}

void deletePerson(Person* obj, Engine* engine)
{
    Engine_Render_deleteObject(engine, obj->mRenderObject);
}
//----//

//--Declare Managers and objects--//
extern const uint16_t people[];

Engine* gEngine;

Person group[5];
//----//

//--Init Function--//
void init()
{   
    gEngine = newEngine();
    Engine_Render_addImage(gEngine, "person", people, 50, 50);
}
//----//

void app_main(void)
{   
    init();

    for(int i = 0; i < 5; i++)
    {
        newPerson(&group[i], gEngine, 20 * i + 1, 20 * i + 1);
        Engine_Render_addObject(gEngine, group[i].mRenderObject);
    }
    Engine_Render_update(gEngine);
    vTaskDelay(1000/portTICK_PERIOD_MS);

    Engine_Render_removeObject(gEngine, group[4].mRenderObject);
    Engine_Render_update(gEngine);
    vTaskDelay(1000/portTICK_PERIOD_MS);

    Engine_Render_addObject(gEngine, group[4].mRenderObject);
    Engine_Render_update(gEngine);
    vTaskDelay(1000/portTICK_PERIOD_MS);

    Engine_Render_clear(gEngine);
    Engine_Render_update(gEngine);
    vTaskDelay(1000/portTICK_PERIOD_MS);

    for(int i = 0; i < 5; i++)
    {
        Engine_Render_addObject(gEngine, group[i].mRenderObject);
    }
    Engine_Render_update(gEngine);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    
    while(1)
    {
        for(int i = 0; i < 5; i++)
        {
            group[i].move(&(group[i]));
        }
        for(int i = 0; i < 5; i++)
        {
            Engine_Render_render(gEngine, group[i].mRenderObject);
        }
        Engine_Render_update(gEngine);
        vTaskDelay(100/portTICK_PERIOD_MS);
    }

    /*
    for(int i = 0; i < 5; i++)
    {
        deletePerson(&group[i], gEngine);
    }
    */
    //printf("%s\n", "Code finished.");
    //return 0;
}
