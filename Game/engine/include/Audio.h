/*
    Audio.h

    Class AudioPlayer and AudioResource.
*/

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <stdint.h>
#include "./Resource.h"

/*
    AudioResource class (derived from Resource), indicate an audio resource.
*/
typedef struct audioResource
{
    Resource* baseResource;
    uint8_t mAudioResourceID;

    //constructor for AudioResource
    void (*new)(struct audioResource*, uint8_t, Resource*);
}AudioResource;

//constructor for AudioResource
void audioResourceNew(AudioResource*, uint8_t, Resource*);

/*
    AudioPlayer class, plays audio.
*/
typedef struct audioPlayer
{
    uint8_t mAudioResourceCount;
    AudioResource mAudioResources[MAX_OBJECT_COUNT];

    //Finds AudioResource by its ID, returns NULL if not found.
    AudioResource* (*findAudioResourceByID)(struct audioPlayer*, uint8_t);
    //Finds RenderResource by its name, returns NULL if not found.
    AudioResource* (*findAudioResourceByName)(struct audioPlayer*, char[]); 

    //Adds AudioResource, whose base class is also registered in the ResourceManager.
    void (*addAudio)(struct audioPlayer*, ResourceManager*, char[]);
    
    //constructor for AudioPlayer
    void (*new)(struct audioPlayer*);

    //Play audio
    void (*play)(struct audioPlayer*, AudioResource*);

    //Stop audio
    void (*stop)(struct audioPlayer*);

    //Pause audio
    void (*pause)(struct audioPlayer*);

    //Resume audio
    void (*resume)(struct audioPlayer*);

    //Set volume
    void (*setVolume)(struct audioPlayer*, int);

    //Handle events
    void (*handleAudioEvents)(struct audioPlayer*);
}AudioPlayer;

AudioPlayer* newAudioPlayer();
void deleteAudioPlayer(AudioPlayer*);
//constructor for AudioPlayer
void audioPlayerNew(AudioPlayer*);

AudioResource* audioPlayerFindAudioResourceByID(AudioPlayer*, uint8_t); 
AudioResource* audioPlayerFindAudioResourceByName(AudioPlayer*, char[]);
void audioPlayerAddAudio(AudioPlayer*, ResourceManager*, char[]); 

void audioPlayerPlay(AudioPlayer*, AudioResource*);
void audioPlayerStop(AudioPlayer*);
void audioPlayerPause(AudioPlayer*);
void audioPlayerResume(AudioPlayer*);
void audioPlayeSetVolume(AudioPlayer*, int);
void audioPlayerHandleAudioEvents(AudioPlayer*);

#endif /* _AUDIO_H_ */