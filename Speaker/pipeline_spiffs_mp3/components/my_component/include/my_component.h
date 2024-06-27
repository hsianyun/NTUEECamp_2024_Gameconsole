#ifndef MY_COMPONENT_H
#define MY_COMPONENT_H

#include "audio_pipeline.h"
#include "esp_peripherals.h"

typedef enum {
    AUDIO_FIRST = 0,
    AUDIO_SECOND,
    AUDIO_NUM
} audio_index_t;

#define RINGBUF_INPUT_SIZE (6 * 1024)

#define AUDIO_INDEX_ITERATE(index) for (int index = 0; index < AUDIO_NUM; index++)

// static const char *TAG = "MY_COMPONENT";

// 函數原型
void initialize_audio_system();

void set_mp3(const char *file_path);

void start_mp3();

void play_mp3();

void handle_audio_events();

void replay_audio();

void pause_audio();

void resume_audio();

void stop_audio();

void set_volume(int volume);

#endif // MY_COMPONENT_H