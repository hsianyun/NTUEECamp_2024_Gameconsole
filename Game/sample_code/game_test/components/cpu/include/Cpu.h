/*
    CPU imitate human's behavior and calculates virtual inputs
*/

#ifndef _CPU_H_
#define _CPU_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "Game.h"

typedef struct game Game;

typedef struct cpu
{
    int8_t keyboardSim;
    int16_t JoystickXSim;
    int16_t JoystickYSim;
    int16_t CurrentGoal; //block level 0, 1, 2, set -1 for no goal

    void (*compute)(struct cpu* obj, Game* game);
    void (*update)(struct cpu* obj);
}CPU;

void cpuCompute(CPU* obj, Game* game);
CPU* newCPU();
void deleteCPU(CPU* obj);

#endif // _CPU_H_
