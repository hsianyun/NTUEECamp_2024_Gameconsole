#ifndef _WEAPON_C_
#define _WEAPON_C_

#include "Weapon.h"
#include "Person.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int possibleLocations[4][2] = {{35, 120}, {235, 120}, {75, 65}, {195, 65}};
const char* weapon_allArray[3][2] =
{
    {"weapon1-l", "weapon1-r"}, {"weapon2-l", "weapon2-r"}, {"weapon3-l", "weapon3-r"}
};

void weaponSetAvailable(Weapon* obj, bool available)
{
    obj->available = available;
}

void weaponDrop(Weapon* obj)
{
    obj->owner = NULL;
    obj->intact = 1;
    obj->intactTime = 10;
    obj->remainTime = 100;
}

bool weaponHit(Weapon* obj, Person* person)
{
    if(obj->owner == NULL)
    {
        if(person->posX + 35 >= obj->posX && person->posX + 15 <= obj->posX + 50
            && person->posY + 50 >= obj->posY + 10 && person->posY <= obj->posY + 40
            && obj->intact == 0)
        {
            obj->owner = person;
            obj->owner->releaseWeapon(obj->owner);
            obj->owner->holdWeapon(obj->owner, obj);
            return true;
        }
        else return false;
    }
    else return false;
}

void weaponUpdate(Weapon* obj, Engine* engine)
{
    if(obj->owner == NULL)
    {
        if(obj->remainTime != 0)
        {
            obj->remainTime--;
            if(obj->intactTime != 0)
            {
                obj->intactTime--;
            }
            else obj->intact = 0;
        }
        else
        {
            obj->available = 0;
        }
    }
    else
    {
        obj->posX = obj->owner->posX;
        obj->posY = obj->owner->posY;
        obj->mRenderObject->setPos(obj->mRenderObject, obj->posX, obj->posY);
        Engine_Render_changeObjectImage(engine, obj->mRenderObject, weapon_allArray[obj->weaponType - 1][obj->owner->oriX >= 0 ? 1 : 0]);
    }
}

Weapon* newWeapon(Engine* engine, uint8_t weaponType, uint8_t loc)
{
    Weapon* obj = calloc(1, sizeof(Weapon));
    obj->loc = loc;
    obj->available = 1;
    obj->remainTime = 100;
    obj->intact = 0;
    obj->intactTime = 0;
    obj->posX = possibleLocations[loc][0];
    obj->posY = possibleLocations[loc][1];
    obj->weaponType = weaponType;
    obj->owner = NULL;

    obj->setAvailable = weaponSetAvailable;
    obj->drop = weaponDrop;
    obj->hit = weaponHit;
    obj->update = weaponUpdate;

    obj->mRenderObject = Engine_Render_newObject(engine, weapon_allArray[weaponType - 1][1], obj->posX, obj->posY, 1);
    return obj;
}

void deleteWeapon(Weapon* obj, Engine* engine)
{
    if(obj != NULL)
    {
        Engine_Render_deleteObject(engine, obj->mRenderObject);
        free(obj);
    }
}

#endif // _WEAPON_C_