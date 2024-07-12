#ifndef KEYBOARDLIB_H
#define KEYBOARDLIB_H

#include <stdbool.h>

#define NUM_OF_COL 3
#define NUM_OF_ROW 2

#define UP_LEFT_KEY     0b00100000
#define UP_CENTER_KEY   0b00010000
#define UP_RIGHT_KEY    0b00001000
#define DOWN_LEFT_KEY   0b00000100
#define DOWN_CENTER_KEY 0b00000010
#define DOWN_RIGHT_KEY  0b00000001
#define NO_KEY          0b00000000
#define ALL_KEYS        0b00111111

void keyboardInit(void);
void detectKeyboard(void);

const uint8_t* registerKeyPress(void);
const uint8_t* registerKeyState(void);
const uint8_t* registerKeyRelease(void);
/*
void printKeyState(void);
void printKeyPress(void);
void printKeyRelease(void);
*/

#endif