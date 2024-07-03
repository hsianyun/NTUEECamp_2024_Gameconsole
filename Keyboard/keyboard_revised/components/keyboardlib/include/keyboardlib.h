#ifndef KEYBOARDLIB_H
#define KEYBOARDLIB_H

#include <stdbool.h>

#define NUM_OF_COL 3
#define NUM_OF_ROW 2

void keyboardSetup(void);
void detectKeyboard(void);
void printKeyState(void);
void printKeyPress(void);
void printKeyRelease(void);

const bool* getKeyState(void);

#endif