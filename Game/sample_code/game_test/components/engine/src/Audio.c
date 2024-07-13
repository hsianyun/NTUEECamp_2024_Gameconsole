/*
    Audio.c

    Functions of Audio.h
*/

#ifndef _AUDIO_C_
#define _AUDIO_C_

#include <string.h>
#include <stdint.h>
#include "./Audio.h"
#include "./Resource.h"

//include library
#include "../../my_board/my_board_v1_0/board.h"
#include "../../my_board/my_codec_driver/new_codec.h"
#include "../../speaker/include/speaker.h"

/*
    AudioResource class
*/
void audioResourceNew(AudioResource* obj, uint8_t ID, Resource* resource)
{
    obj->new = audioResourceNew;
    obj->mAudioResourceID = ID;
    obj->baseResource = resource;
}

/*
    AudioPlayer class
*/
AudioPlayer* newAudioPlayer()
{
    AudioPlayer* audioPlayer = calloc(1, sizeof(AudioPlayer));
    audioPlayerNew(audioPlayer);
    return audioPlayer;
}

void deleteAudioPlayer(AudioPlayer* obj)
{
    terminate_audio();
    if(obj != NULL) free(obj);
}

void audioPlayerNew(AudioPlayer* obj)
{
    obj->mAudioResourceCount = 0;
    obj->new = audioPlayerNew;
    obj->findAudioResourceByID = audioPlayerFindAudioResourceByID;
    obj->findAudioResourceByName = audioPlayerFindAudioResourceByName;
    obj->addAudio = audioPlayerAddAudio;
    obj->play = audioPlayerPlay;
    obj->stop = audioPlayerStop;
    obj->pause = audioPlayerPause;
    obj->resume = audioPlayerResume;
    obj->setVolume = audioPlayeSetVolume;
    obj->handleAudioEvents = audioPlayerHandleAudioEvents;
    initialize_audio_system();
    return;
}

AudioResource* audioPlayerFindAudioResourceByID(AudioPlayer* obj, uint8_t ID)
{
    if(ID < obj->mAudioResourceCount)
    {
        return &(obj->mAudioResources[ID]);
    }
    else return NULL;
}

AudioResource* audioPlayerFindAudioResourceByName(AudioPlayer* obj, char name[])
{
    for(uint8_t i = 0; i < obj->mAudioResourceCount; i++)
    {
        if(strcmp(((obj->mAudioResources[i]).baseResource)->mFileName, name) == 0)
        {
            return &(obj->mAudioResources[i]);
        }
    }
    return NULL;
}

void audioPlayerAddAudio(AudioPlayer* obj, ResourceManager* resourceManager, char name[])
{
    resourceManager->addResource(resourceManager, name, RESOURCE_AUDIO);
    audioResourceNew(&obj->mAudioResources[obj->mAudioResourceCount], obj->mAudioResourceCount, resourceManager->findResourceByID(resourceManager, (resourceManager->mResourceCount) - 1));
    obj->mAudioResourceCount++;
    return;
}

void audioPlayerPlay(AudioPlayer* obj, AudioResource* audioResource)
{
    set_audio(audioResource->baseResource->mFileName);
    play_audio();
}

void audioPlayerStop(AudioPlayer* obj)
{
    stop_audio();
}

void audioPlayerPause(AudioPlayer* obj)
{
    pause_audio();
}

void audioPlayerResume(AudioPlayer* obj)
{
    resume_audio();
}

void audioPlayeSetVolume(AudioPlayer* obj, int volume)
{
    set_volume(volume);
}

void audioPlayerHandleAudioEvents(AudioPlayer* obj)
{
    handle_audio_events();
}

#endif /* _AUDIO_C_ */