#include <stdio.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiffs.h"

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
    Engine_Audio_addAudio(gEngine, "/spiffs/adf_music.mp3");
    Engine_Audio_addAudio(gEngine, "/spiffs/gunshot.mp3");
    Engine_Audio_addAudio(gEngine, "/spiffs/metalpipe.mp3");
    Engine_Audio_addAudio(gEngine, "/spiffs/potai.mp3");
    Engine_Audio_addAudio(gEngine, "/spiffs/yamete.mp3");
    Engine_Audio_addAudio(gEngine, "/spiffs/guncock.mp3");
    Engine_Audio_setVolume(gEngine, 0);
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
    
    while(1)
    {
        Engine_Detect(gEngine);
        Engine_Audio_handleAudioEvents(gEngine);

        if(Engine_Keyboard_isKeyPress(gEngine, 0b000001))
        {
            Engine_Audio_play(gEngine, "/spiffs/gunshot.mp3");
        }
        else if(Engine_Keyboard_isKeyPress(gEngine, 0b000010))
        {
            Engine_Audio_play(gEngine, "/spiffs/adf_music.mp3");
        }
        else if(Engine_Keyboard_isKeyPress(gEngine, 0b000100))
        {
            Engine_Audio_play(gEngine, "/spiffs/metalpipe.mp3");
        }
        else if(Engine_Keyboard_isKeyPress(gEngine, 0b001000))
        {
            Engine_Audio_play(gEngine, "/spiffs/potai.mp3");
        }
        else if(Engine_Keyboard_isKeyPress(gEngine, 0b010000))
        {
            Engine_Audio_play(gEngine, "/spiffs/yamete.mp3");
        }
        else if(Engine_Keyboard_isKeyPress(gEngine, 0b100000))
        {
            Engine_Audio_play(gEngine, "/spiffs/guncock.mp3");
        }

        
        for(int i = 0; i < 5; i++)
        {
            group[i].move(&(group[i]));
        }
        for(int i = 0; i < 5; i++)
        {
            Engine_Render_render(gEngine, group[i].mRenderObject);
        }
        Engine_Render_update(gEngine);
        vTaskDelay((1000/60)/portTICK_PERIOD_MS);
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
