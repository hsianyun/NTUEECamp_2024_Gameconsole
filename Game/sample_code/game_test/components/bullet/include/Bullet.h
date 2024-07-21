#ifndef _BULLET_H_
#define _BULLET_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "Engine.h"
#include "Person.h"

typedef struct person Person;

typedef struct bullet
{
    RenderObject *mRenderObject;
    int speed;
    int angle; //0~360
    int posX;
    int posY;

    //void (*new)(struct person*, struct bullet*, int16_t);
    bool (*move)(struct bullet*);
    void (*update)(struct bullet*);
}Bullet;


bool bulletMove(Bullet* obj);// return whether the bullet is alive
void bulletUpdate(Bullet* obj);
//void bulletNew(Person* person, Bullet* obj, int16_t angle); // create a bullet from the position of a person
Bullet* newBullet(Engine* engine, Person* person, int16_t angle);
void deleteBullet(Bullet* obj, Engine* engine);

#endif // _BULLET_H_
