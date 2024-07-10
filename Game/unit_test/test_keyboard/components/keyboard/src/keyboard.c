#include "keyboard.h"
#include "keyboardlib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

Keyboard* newKeyboard()
{
    keyboardInit();
    Keyboard *keyboard = calloc(1, sizeof(Keyboard));
    keyboard->keyPress = registerKeyPress();
    keyboard->keyState = registerKeyState();
    keyboard->keyRelease = registerKeyRelease();
    return keyboard;
}

void deleteKeyboard(Keyboard *keyboard)
{
    free(keyboard);
}

void Keyboard_Detect(Keyboard *keyboard)
{
    detectKeyboard();
}

uint8_t Keyboard_getKeyDown(Keyboard *keyboard)
{
    return *(keyboard->keyState);
}

uint8_t Keyboard_getKeyPress(Keyboard *keyboard)
{
    return *(keyboard->keyPress);
}

uint8_t Keyboard_getKeyRelease(Keyboard *keyboard)
{
    return *(keyboard->keyRelease);
}

bool Keyboard_isKeyDown(Keyboard *keyboard, uint8_t key)
{
    return (~*(keyboard->keyState) & key) == 0;
}

bool Keyboard_isKeyPress(Keyboard *keyboard, uint8_t key)
{
    return (~*(keyboard->keyPress) & key) == 0;
}

bool Keyboard_isKeyRelease(Keyboard *keyboard, uint8_t key)
{
    return (~*(keyboard->keyRelease) & key) == 0;
}