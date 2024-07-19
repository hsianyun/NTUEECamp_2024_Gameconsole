#ifndef _TREASURE_C_
#define _TREASURE_C_

#include "Treasure.h"
#include "Person.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void treasureSetAvailable(Treasure* obj, bool available)
{
    obj->available = available;
    if(obj->available) obj->remainTime = 150;
}

bool treasureHit(Treasure* obj, Person* person)
{
    if(obj->available)
    {
        if(person->posX + 35 >= obj->posX && person->posX + 15 <= obj->posX + 50
            && person->posY + 50 >= obj->posY + 10 && person->posY <= obj->posY + 40)
        {
            obj->available = 0;
            person->obtainItem(person);
            return true;
        }
        else return false;
    }
    else return false;
}

void treasureUpdate(Treasure* obj)
{
    if(obj->remainTime != 0) 
    {
        obj->remainTime--;        
    }
    else
    {
        obj->available = 0;
    }
    obj->mRenderObject->setVisible(obj->mRenderObject, obj->available);
}

Treasure* newTreasure(Engine* engine)
{
    Treasure* obj = calloc(1, sizeof(Treasure));
    obj->available = 0;
    obj->posX = 135;
    obj->posY = 20;

    obj->setAvailable = treasureSetAvailable;
    obj->hit = treasureHit;
    obj->update = treasureUpdate;

    obj->mRenderObject = Engine_Render_newObject(engine, "treasure", obj->posX, obj->posY, 0);
    return obj;
}

void deleteTreasure(Treasure* obj, Engine* engine)
{
    if(obj != NULL)
    {
        Engine_Render_deleteObject(engine, obj->mRenderObject);
        free(obj);
    }
}

#endif // _TREASURE_C_