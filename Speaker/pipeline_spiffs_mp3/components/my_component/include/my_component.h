#ifndef MY_COMPONENT_H
#define MY_COMPONENT_H

#include "audio_pipeline.h"
#include "esp_peripherals.h"

// static const char *TAG = "MY_COMPONENT";

// functions
// 初始化音頻系統
void initialize_audio_system();

// 設置播放檔案
void set_mp3(const char *file_path);

// 啟動播放系統
void start_mp3();

// 播放
void play_mp3();

// 隨時處理音頻播放狀態
// 當音樂結束時 replay_audio();
void handle_audio_events();

// 打斷現在音樂進行並重複播放
void replay_audio();

// 暫停
void pause_audio();

// 恢復播放
void resume_audio();

// 停止播放並銷毀現有進程
void stop_audio();

// 設置音量大小
// volume 範圍有待商榷
void set_volume(int volume);

#endif // MY_COMPONENT_H