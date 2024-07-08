#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_common.h"
#include "esp_peripherals.h"
#include "periph_spiffs.h"
#include "spiffs_stream.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"
#include "string.h"
#include "driver/i2s_std.h"
#include "speaker.h"

// audio_pipeline_handler 將個別處理部分整合
static audio_pipeline_handle_t play_pipeline;

// audio_elements_handler 控制每個個別的音訊處理組成
// 含有 spiffs/mp3_decoder/i2s_stream_writer
static audio_element_handle_t spiffs_stream_reader;
static audio_element_handle_t audio_decoder;
static audio_element_handle_t i2s_stream_writer;

// esp_peripheral_handler，在此處理 spiffs
static esp_periph_set_handle_t set;

// audio_event_handle 處理音訊播放
static audio_event_iface_handle_t evt;

// audio_event_message 處理音訊狀態
static audio_event_iface_msg_t msg;

// initialize_audio_system
void initialize_audio_system() {

    // initialize esp_peripheral
    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
    set = esp_periph_set_init(&periph_cfg);

    periph_spiffs_cfg_t spiffs_cfg = {
        .root = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };
    esp_periph_handle_t spiffs_handle = periph_spiffs_init(&spiffs_cfg);

    // start spiffs
    esp_periph_start(set, spiffs_handle);

    // Wait until spiffs is mounted
    while (!periph_spiffs_is_mounted(spiffs_handle)) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    // Initialize pipeline
    audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
    play_pipeline = audio_pipeline_init(&pipeline_cfg);
    mem_assert(play_pipeline);

    // Initialize spiffs_stream
    spiffs_stream_cfg_t flash_cfg = SPIFFS_STREAM_CFG_DEFAULT();
    flash_cfg.type = AUDIO_STREAM_READER;
    spiffs_stream_reader = spiffs_stream_init(&flash_cfg);

    // Initialize mp3_decoder
    mp3_decoder_cfg_t mp3_cfg = DEFAULT_MP3_DECODER_CONFIG();
    audio_decoder = mp3_decoder_init(&mp3_cfg);

    // Initialize i2s_stream
    i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT();
    i2s_cfg.type = AUDIO_STREAM_WRITER;
    i2s_cfg.use_alc = true;
    // 設置初始音量大小
    i2s_cfg.volume = 0;
    i2s_stream_writer = i2s_stream_init(&i2s_cfg);

    // Regist audio pipeline
    audio_pipeline_register(play_pipeline, spiffs_stream_reader, "file");
    audio_pipeline_register(play_pipeline, audio_decoder, "mp3");
    audio_pipeline_register(play_pipeline, i2s_stream_writer, "i2s");
    const char *link_tag[3] = {"file", "mp3", "i2s"};
    audio_pipeline_link(play_pipeline, &link_tag[0], 3);

    // 初始化音訊播放處理系統
    audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    evt = audio_event_iface_init(&evt_cfg);

    // set_listner for pipeline
    audio_pipeline_set_listener(play_pipeline, evt);
}

// 設置 mp3 文件，選擇音檔
void set_audio(const char *file_path) {
    printf("Success : set_audio()\n");
    audio_element_set_uri(spiffs_stream_reader, file_path);
}

// 播放 mp3
void play_audio() {
    printf("Success : play_audio()\n");
    audio_pipeline_run(play_pipeline);
}

// 處理音訊播放
void handle_audio_events()
{
    // Read message (wait 100 ms)
    esp_err_t ret = audio_event_iface_listen(evt, &msg, 100/portTICK_PERIOD_MS);

    // If fail to read, continue
    if (ret != ESP_OK) {
        return;
    }

    // Modify audio data
    if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.cmd == AEL_MSG_CMD_REPORT_MUSIC_INFO) {
        printf("Audio info is modified :\n");
        audio_element_info_t music_info = {0};
        static audio_element_info_t prev_music_info = {0};
        audio_element_getinfo(audio_decoder, &music_info);
        if ((prev_music_info.bits != music_info.bits) || (prev_music_info.sample_rates != music_info.sample_rates)
            || (prev_music_info.channels != music_info.channels)) {
            i2s_stream_set_clk(i2s_stream_writer, music_info.sample_rates, music_info.bits, music_info.channels);
            memcpy(&prev_music_info, &music_info, sizeof(audio_element_info_t));
        }
        printf("sample_rates = %d, bit depth = %d\n", music_info.sample_rates, music_info.bits);
        return;
    }

    // When finish playing, stop the pipeline and reset, wait for the next run
    if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.source == (void *) audio_decoder && msg.cmd == AEL_MSG_CMD_REPORT_STATUS &&
        ((int)msg.data == AEL_STATUS_STATE_FINISHED)) {
        printf("Audio is finished\n");
        audio_pipeline_stop(play_pipeline);
        audio_pipeline_wait_for_stop(play_pipeline);
        audio_pipeline_terminate(play_pipeline);
        audio_pipeline_reset_ringbuffer(play_pipeline);
        audio_pipeline_reset_elements(play_pipeline);
        audio_pipeline_change_state(play_pipeline, AEL_STATE_INIT);
    }
}

// 獲取音檔資訊
int get_audio_state()
{
    // return value comes from the data of i2s_stream_writer
    return audio_element_get_state(i2s_stream_writer);
}

// 暫停播放
void pause_audio() {
    if(msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.cmd == AEL_MSG_CMD_REPORT_STATUS &&
        (((int)msg.data == AEL_STATUS_STATE_RUNNING)))
        {
            printf("Success : pause_audio()\n");
            audio_pipeline_pause(play_pipeline);
        }
    else {
        printf("Error : pause_audio() didn't work\n");
    }
}

// 恢復播放
void resume_audio() {
    if(msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.cmd == AEL_MSG_CMD_REPORT_STATUS &&
        (((int)msg.data == AEL_STATUS_STATE_PAUSED || (int)msg.data == AEL_STATUS_STATE_STOPPED))) {
        printf("Success : resume_audio()\n");
        audio_pipeline_resume(play_pipeline);
    }
    else {
        printf("Error : resume_audio() didn't work\n");
    }
}

// 停止播放
void stop_audio() {
    if(get_audio_state() == AEL_STATE_RUNNING || get_audio_state() == AEL_STATE_PAUSED) {
        printf("Success : stop_audio()\n");
        audio_pipeline_stop(play_pipeline);
        audio_pipeline_wait_for_stop(play_pipeline);
        audio_pipeline_terminate(play_pipeline);
        audio_pipeline_reset_ringbuffer(play_pipeline);
        audio_pipeline_reset_elements(play_pipeline);
        audio_pipeline_change_state(play_pipeline, AEL_STATE_INIT);
    }
    else {
        printf("Error : stop_audio() didn't work\n");
    }
}

// 停止播放並銷毀
void terminate_audio() {
    
    audio_pipeline_stop(play_pipeline);
    audio_pipeline_wait_for_stop(play_pipeline);
    audio_pipeline_terminate(play_pipeline);
    audio_pipeline_unregister(play_pipeline, spiffs_stream_reader);
    audio_pipeline_unregister(play_pipeline, audio_decoder);
    audio_pipeline_unregister(play_pipeline, i2s_stream_writer);
    audio_pipeline_remove_listener(play_pipeline);
    audio_pipeline_deinit(play_pipeline);
    audio_element_deinit(audio_decoder);
    audio_element_deinit(spiffs_stream_reader);
    audio_element_deinit(i2s_stream_writer);

    esp_periph_set_stop_all(set);
    audio_event_iface_remove_listener(esp_periph_set_get_event_iface(set), evt);
    audio_event_iface_destroy(evt);
    esp_periph_set_destroy(set);
}

// 設置音量大小
void set_volume(int volume)
{
    i2s_alc_volume_set(i2s_stream_writer, volume);
}

