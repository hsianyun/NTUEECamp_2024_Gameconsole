#ifndef _BULLET_C_
#define _BULLET_C_

#include "Bullet.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define PI 3.14159265

//Moves the bullet, and return whether the bullet is alive
bool bulletMove(Bullet* obj)
{
    obj->posX += cos(obj->angle * PI / 180) * obj->speed;
    obj->posY -= sin(obj->angle * PI / 180) * obj->speed;
    
    if(obj->posX<0 || obj->posY<0 || obj->posX>320 || obj->posY > 240)
    {
        return false;
    }
    return true;
}

void bulletUpdate(Bullet* obj)
{
    obj->mRenderObject->setPos(obj->mRenderObject, obj->posX, obj->posY);
}

Bullet* newBullet(Engine* engine, Person* person, int16_t angle)
{
    Bullet* obj = calloc(1, sizeof(Bullet));
    obj->move = bulletMove;
    obj->update = bulletUpdate;

     //weapon
    //1->pistol speed=10 cd = 4 angle = 0 or 180
    //2->shotgun speed = 15 cd = 10 angle = 0, 30, -30 or 180 210 -210 (3*bullet)
    //3->submachine speed = 20 cd = 2 angle = 0 or 180
    obj->posX = person->posX + 25;
    obj->posY = person->posY + 25;
    obj->angle = angle;

    switch (person->weapon_type)
    {
        case 2:
            obj->speed = 15;
            break;
        case 3: 
            obj->speed = 20; 
            break;
        default:
            obj->speed = 10;
            break;
    }

    obj->mRenderObject = Engine_Render_newObject(engine, "bullet", obj->posX, obj->posY, 1);
    return obj;
}

void deleteBullet(Bullet* obj, Engine* engine)
{
    if(obj != NULL)
    {
        Engine_Render_deleteObject(engine, obj->mRenderObject);
        free(obj);
    }
}

#endif // _BULLET_C_