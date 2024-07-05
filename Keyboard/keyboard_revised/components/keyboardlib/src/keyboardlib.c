#include <driver/gpio.h>
#include "keyboardlib.h"
#include <time.h>

#define GPIO_HIGH 1
#define GPIO_LOW 0

const uint8_t ROW[NUM_OF_ROW] = {GPIO_NUM_32, GPIO_NUM_33};      // Array for row pins
const uint8_t COLUMN[NUM_OF_COL] = {GPIO_NUM_14, GPIO_NUM_12, GPIO_NUM_13};  // Array for column pins

bool mem1[NUM_OF_ROW*NUM_OF_COL] = {0}, mem2[NUM_OF_ROW*NUM_OF_COL] = {0};
bool *curr_key_state = mem1, *last_key_state = mem2; // Array to store key states in current and last frame. 0 = not pressed, 1 = pressed
bool keyState[NUM_OF_ROW*NUM_OF_COL] = {0}, keyPress[NUM_OF_ROW*NUM_OF_COL] = {0}, keyRelease[NUM_OF_ROW*NUM_OF_COL] = {0};
clock_t timeOfChange[NUM_OF_ROW*NUM_OF_COL] = {0}; // Debouncing: array to store latest time of input change
bool bouncing[NUM_OF_ROW*NUM_OF_COL] = {0}; // Array to store whether key is bouncing
const uint8_t ULT_CODE[ULT_NUM][ULT_LEN] = {{0, 2, 3, 4, 1},
                                            {1, 1, 4, 4, 2}}; // The codes to trigger ultimate skills
int ultSkillExecute = -1; // the ultimate skill to be executed

// Used in detectKeyboard() to handle changes of ultimate skill detection by key press
void handleUltimateSkill(void){
    // Set ultSkillExecute to be the index of ultimate skill triggered. -1 for no combination satisfied.
    static uint8_t ultCount[ULT_NUM] = {0}; // Array to store current step of each ultimate skill
    static clock_t lastTimeKeyPress = 0;
    for(uint8_t i = 0; i < ULT_NUM; i++){
        if(1000 * (clock() - lastTimeKeyPress) > ULT_DELAY_MS * CLOCKS_PER_SEC){
            // click too slow
            ultCount[i] = 0;
        }
        else{
            bool keyPressBuffer[NUM_OF_ROW*NUM_OF_COL]; // store key pressed
            for(uint8_t j = 0; j < NUM_OF_ROW*NUM_OF_COL; j++){
                keyPressBuffer[j] = (curr_key_state[j] > last_key_state[j]);
            }
            /*Current design for multiple keys pressed at the same time: 
                If any order of all the keys pressed (not repeated) correspond to
                some of the next keys for an ultimate skill, or any order of some
                keys pressed (not repeated) correspond to the rest of an ultimate
                skill code, then consider a successful click of those keys.
                Fails otherwise (ultCount reset to 0).
            */
            while(keyPressBuffer[ULT_CODE[i][ultCount[i]]]){
                // current skill code step satisfied, check next one
                keyPressBuffer[ULT_CODE[i][ultCount[i]]] = 0;
                ultCount[i]++;
                if(ultCount[i] == ULT_LEN){
                    // successfully triggers the ultimate skill
                    ultSkillExecute = i;
                    ultCount[i] = 0;
                    break;
                }
            }
            for(uint8_t j = 0; j < NUM_OF_ROW*NUM_OF_COL; j++){
                if(keyPressBuffer[j] && ultSkillExecute == -1){
                    // fails, redetect from ultCount[i] = 0
                    ultCount[i] = 0;
                    for(uint8_t j = 0; j < NUM_OF_ROW*NUM_OF_COL; j++){
                        keyPressBuffer[j] = (curr_key_state[j] > last_key_state[j]);
                    }
                    while(keyPressBuffer[ULT_CODE[i][ultCount[i]]]){
                        // current skill code step satisfied, check next one
                        keyPressBuffer[ULT_CODE[i][ultCount[i]]] = 0;
                        ultCount[i]++;
                    }
                    for(uint8_t j = 0; j < NUM_OF_ROW*NUM_OF_COL; j++){
                        if(keyPressBuffer[j]){
                            // fails
                            ultCount[i] = 0;
                        }
                    }
                    break;
                }
            }
        }
    }
    if(ultSkillExecute != -1){
        // a ultimate skill triggered
        for(uint8_t i = 0; i < ULT_NUM; i++){
            ultCount[i] = 0;
        }
    }
    
    for(uint8_t i = 0; i < NUM_OF_ROW*NUM_OF_COL; i++){
        if(curr_key_state[i] > last_key_state[i]){
            // some key pressed
            lastTimeKeyPress = clock();
            break;
        }
    }
}

void keyboardSetup(void){
    // Initialize row pins as outputs and set them to HIGH
    for(uint8_t i = 0; i < NUM_OF_ROW; i++){
        gpio_set_direction(ROW[i], GPIO_MODE_OUTPUT);    // Set pin direction to OUTPUT
        gpio_set_level(ROW[i], 1);   // Set initial level to HIGH (active)
    }

    // Initialize column pins as inputs with pull-up resistors enabled
    for(uint8_t i = 0; i < NUM_OF_COL; i++){
        gpio_set_direction(COLUMN[i], GPIO_MODE_INPUT);   // Set pin direction to INPUT
        gpio_set_pull_mode(COLUMN[i], GPIO_PULLUP_ONLY);  // Enable pull-up resistor
    }
}

// Should be called by the main loop to detect key presses in each frame
void detectKeyboard(void){

    // Swap the current and last key state arrays
    bool *tmp = last_key_state;
    last_key_state = curr_key_state;
    curr_key_state = tmp;
    for(uint8_t i = 0; i < NUM_OF_ROW; i++){
        gpio_set_level(ROW[i], GPIO_LOW);   // Activate the current row (set LOW)

        // Iterate through each column to check key status
        for(uint8_t j = 0; j < NUM_OF_COL; j++){
            curr_key_state[i*NUM_OF_COL + j] = !gpio_get_level(COLUMN[j]);  // Set the key state in the current frame
        }

        gpio_set_level(ROW[i], GPIO_HIGH);   // Deactivate the current row (set HIGH)
    }
    for(uint8_t i = 0; i < NUM_OF_ROW*NUM_OF_COL; i++){
        keyState[i] = curr_key_state[i];
        keyPress[i] = (curr_key_state[i] > last_key_state[i]);
        keyRelease[i] = (curr_key_state[i] < last_key_state[i]);
    }
    handleUltimateSkill();
}

// Test
void printKeyState(void){
    printf("Key state:\n");
    for(uint8_t i = 0; i < NUM_OF_ROW; i++){
        for(uint8_t j = 0; j < NUM_OF_COL; j++){
            printf("%d ", curr_key_state[i*NUM_OF_COL + j]);
        }
        printf("\n");
    }
    //printf("\n");
}

void printKeyPress(void){
    printf("Key press:\n");
    for(uint8_t i = 0; i < NUM_OF_ROW; i++){
        for(uint8_t j = 0; j < NUM_OF_COL; j++){
            printf("%d ", curr_key_state[i*NUM_OF_COL + j] > last_key_state[i*NUM_OF_COL + j]);
        }
        printf("\n");
    }
    //printf("\n");
}

void printKeyRelease(void)
{
    printf("Key release:\n");
    for(uint8_t i = 0; i < NUM_OF_ROW; i++){
        for(uint8_t j = 0; j < NUM_OF_COL; j++){
            printf("%d ", curr_key_state[i*NUM_OF_COL + j] < last_key_state[i*NUM_OF_COL + j]);
        }
        printf("\n");
    }
    //printf("\n");
}

// return the index of ultimate skill to be executed and set ultSkillExecute back to -1
int getUltimateSkill(void){
    int ult = ultSkillExecute;
    ultSkillExecute = -1;
    return ult;
}

const bool* getKeyState(void){
    return keyState;
}

const bool* getKeyPress(void){
    return keyPress;
}

const bool* getKeyRelease(void){
    return keyRelease;
}