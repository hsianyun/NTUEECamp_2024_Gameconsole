#ifndef _PERSON_H_
#define _PERSON_H_

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "Weapon.h"
#include "Engine.h"
#include "Bullet.h"
#include "Game.h"

#define FPS 15
#define POWERTIME_DURATION 100
#define HURTTIME_DURATION 10

typedef struct weapon Weapon;
typedef struct bullet Bullet;
typedef struct connectionData ConnectionData;

typedef struct person
{
    RenderObject *mRenderObject;
    RenderObject *mItemObject;
    //RenderObject *mWeaponObject;
    Weapon *mWeapon;

    //No. of the person (1: master, 2: slave)
    uint8_t index;

    // int attack;
    uint8_t HP;
    uint8_t state; //1->normal 2->damaged >=3->invincible (decaded by frames)
    uint8_t hurtTime;
    int8_t hurtPenalty;
    uint8_t powerTime;
    int8_t oriX;
    int16_t oriY;
    int16_t posX;
    int16_t posY;
    int16_t speedX;
    int16_t speedY;
    int16_t accel;
    int8_t weapon_type; // 1->pistol 2->shotgun 3->submachine 4->none
    int8_t cd;// frames

    bool item;

    void (*move)(struct person*, int16_t);
    void (*jump)(struct person*);
    void (*update)(struct person*, Engine*, int); //to update the state of a person.(cd--)
    void (*updateData)(struct person*, Engine*, ConnectionData*, int);
    void (*attack)(struct person*, Bullet*[], Engine*, double angle);
    bool (*damage)(struct person*, Bullet*);
    void (*holdWeapon)(struct person*, Weapon*);
    void (*releaseWeapon)(struct person*);
    void (*dropWeapon)(struct person*);
    void (*obtainItem)(struct person*);
    void (*useItem)(struct person*);
}Person;


//void personMove(Person* obj, int speed_x, bool jump, bool downstair);//speed_x -> x data of joystick
void personMove(Person* obj, int16_t magX);//speed_x -> x data of joystick
void personUpdate(Person* obj, Engine* engine, int frames);
void personUpdateByData(Person* obj, Engine* engine, ConnectionData* data, int frames);
void personAttack(Person* obj, Bullet* bullets[], Engine* engine, double angle);
bool personDamage(Person* obj, Bullet* bullet);
void personHoldWeapon(Person* obj, Weapon* weapon);
void personReleaseWeapon(Person* obj);
void personDropWeapon(Person* obj);
void personObtainItem(Person* obj);
void personUseItem(Person* obj);
Person* newPerson(Engine* engine, int16_t posX, int16_t posY, uint8_t index);
void deletePerson(Person* obj, Engine* engine);

#endif // _PERSON_H_
