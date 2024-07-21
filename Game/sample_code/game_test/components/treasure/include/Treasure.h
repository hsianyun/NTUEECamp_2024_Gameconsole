#ifndef _TREASURE_H_
#define _TREASURE_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "Engine.h"
#include "Person.h"

typedef struct treasure
{
    RenderObject *mRenderObject;
    bool available;
    uint8_t remainTime;

    uint16_t posX;
    uint16_t posY;

    void (*setAvailable)(struct treasure*, bool);
    bool (*hit)(struct treasure*, Person*);
    void (*update)(struct treasure*);
}Treasure;

void treasureSetAvailable(Treasure* obj, bool available);
bool treasureHit(Treasure* obj, Person* person);
void treasureUpdate(Treasure* obj);
Treasure* newTreasure(Engine* engine);
void deleteTreasure(Treasure* obj, Engine* engine);

#endif // _TREASURE_H_
