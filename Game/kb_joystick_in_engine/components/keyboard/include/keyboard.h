#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_OF_COL 3
#define NUM_OF_ROW 2

//the combination of keys are represented by 6 bits
//the combination like up_left + down_right is 0b00100001
#define UP_LEFT_KEY     0b00100000
#define UP_CENTER_KEY   0b00010000
#define UP_RIGHT_KEY    0b00001000
#define DOWN_LEFT_KEY   0b00000100
#define DOWN_CENTER_KEY 0b00000010
#define DOWN_RIGHT_KEY  0b00000001
#define NO_KEY          0b00000000
#define ALL_KEYS        0b00111111

typedef struct keyboard
{
    const uint8_t *keyPress;
    const uint8_t *keyState;
    const uint8_t *keyRelease;
    bool (*isKeyPress)(uint8_t key);
    bool (*isKeyRelease)(uint8_t key);
} Keyboard;

Keyboard* newKeyboard();
void deleteKeyboard(Keyboard *keyboard);
void Keyboard_Detect(Keyboard *keyboard);
//get the key combination of down, just press, just release respectively
uint8_t Keyboard_getKeyDown(Keyboard *keyboard);
uint8_t Keyboard_getKeyPress(Keyboard *keyboard);
uint8_t Keyboard_getKeyRelease(Keyboard *keyboard);
//check if the current down key include given combination
/*ex: if now down key is up_left, up_center, and down_right, 
then isKeyDown(0b00100000), isKeyDown(0b00010000), isKeyDown(0b00000001), isKeyDown(0b00110000), isKeyDown(0b0010001), etc are true
*/
bool Keyboard_isKeyDown(Keyboard *keyboard, uint8_t key);
//check if the keys just pressed in current frame include given combination
/*ex: if up_left, up_center are just pressed in this frame,
then isKeyPress(0b00100000), isKeyPress(0b00010000), isKeyPress(0b00110000) are true
*/
bool Keyboard_isKeyPress(Keyboard *keyboard, uint8_t key);
//check if the keys just released in current frame include given combination
//also follow the rule in isKeyDown and isKeyPress
bool Keyboard_isKeyRelease(Keyboard *keyboard, uint8_t key);

//keyboard.h
#endif