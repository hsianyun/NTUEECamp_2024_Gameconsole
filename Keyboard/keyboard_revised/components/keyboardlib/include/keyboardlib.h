#ifndef KEYBOARDLIB_H
#define KEYBOARDLIB_H

#include <stdbool.h>

#define NUM_OF_COL 3
#define NUM_OF_ROW 2
#define BOUNCE_DELAY_MS 40 // For debouncing
#define ULT_DELAY_MS 1000 // maximum delay between keys of a ultimate skill 
#define ULT_LEN 5 // length of ultimate skills
#define ULT_NUM 2 // amount of ultimate skills

void keyboardSetup(void);
void detectKeyboard(void);
void printKeyState(void);
void printKeyPress(void);
void printKeyRelease(void);
int getUltimateSkill(void);

const bool* getKeyState(void);

#endif