#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiffs.h"
#include "esp_heap_caps.h"

#include "Game.h"



// #include "../files/people.h"


//--important variable--//
Game game;
//----//

//--Declare Managers and objects--//
// extern const uint16_t people[];
// extern const uint16_t people2[];

//----//

void app_main(void)
{   
    gameNew(&game);
    game.init(&game);
    printf("Game init success!!!\n");
    //game.start(&game);

    while(1){
        game.readInput(&game);
        //printf("Game readInput success!!!\n");
        game.update(&game);
        //printf("Game update success!!!\n");
        game.render(&game);
        //printf("Game render success!!!\n");
        //vTaskDelay(2000/portTICK_PERIOD_MS);
        vTaskDelay((2000/60)/portTICK_PERIOD_MS);
    }

    // for(int i = 0; i < 1; i++)
    // {
    //     newPerson(&group[i], gEngine, 1, 190);
    //     Engine_Render_addObject(gEngine, group[i].mRenderObject);
    // }
    // for(int i = 0; i < 2; i++)
    // {
    //     Engine_Render_changeObjectImage(gEngine, group[i].mRenderObject, "person2");
    // }

    // int16_t frame_count = 0;

    //while(0){
        // Engine_Detect(gEngine);
        // Engine_Audio_handleAudioEvents(gEngine);

        // group[0].move(&(group[0]));
        // Engine_Render_render(gEngine, group[0].mRenderObject);
        // if(frame_count%4==1){
        //     Engine_Render_changeObjectImage(gEngine, group[0].mRenderObject, "person2");
        //     Engine_Render_render(gEngine, group[0].mRenderObject);
        // }
        // if(frame_count%4==3){
        //     Engine_Render_changeObjectImage(gEngine, group[0].mRenderObject, "person1");
        //     Engine_Render_render(gEngine, group[0].mRenderObject);
        // }

        // Engine_Render_update(gEngine);
        // vTaskDelay((2000/60)/portTICK_PERIOD_MS);
        // frame_count ++;
    //}

    
    // while(1)
    // {
    //     Engine_Detect(gEngine);
    //     Engine_Audio_handleAudioEvents(gEngine);

    //     if(Engine_Keyboard_isKeyPress(gEngine, 0b000001))
    //     {
    //         Engine_Audio_play(gEngine, "/spiffs/gunshot.mp3");
    //     }
    //     else if(Engine_Keyboard_isKeyPress(gEngine, 0b000010))
    //     {
    //         Engine_Audio_play(gEngine, "/spiffs/adf_music.mp3");
    //     }
    //     else if(Engine_Keyboard_isKeyPress(gEngine, 0b000100))
    //     {
    //         Engine_Audio_play(gEngine, "/spiffs/metalpipe.mp3");
    //     }
    //     else if(Engine_Keyboard_isKeyPress(gEngine, 0b001000))
    //     {
    //         Engine_Audio_play(gEngine, "/spiffs/potai.mp3");
    //     }
    //     else if(Engine_Keyboard_isKeyPress(gEngine, 0b010000))
    //     {
    //         Engine_Audio_play(gEngine, "/spiffs/yamete.mp3");
    //     }
    //     else if(Engine_Keyboard_isKeyPress(gEngine, 0b100000))
    //     {
    //         Engine_Audio_play(gEngine, "/spiffs/guncock.mp3");
    //     }

        
    //     for(int i = 0; i < 5; i++)
    //     {
    //         group[i].move(&(group[i]));
    //     }
    //     for(int i = 0; i < 5; i++)
    //     {
    //         Engine_Render_render(gEngine, group[i].mRenderObject);
    //     }
    //     Engine_Render_update(gEngine);
    //     vTaskDelay((1000/60)/portTICK_PERIOD_MS);
    // }

    /*
    for(int i = 0; i < 5; i++)
    {
        deletePerson(&group[i], gEngine);
    }
    */
    //printf("%s\n", "Code finished.");
    //return 0;
}
