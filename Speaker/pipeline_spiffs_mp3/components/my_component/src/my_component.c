#include <stdlib.h>
#include "my_component.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_common.h"
#include "esp_log.h"
#include "esp_err.h"
#include "periph_spiffs.h"
#include "spiffs_stream.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// audio_pipeline_handler 將個別處理部分整合
static audio_pipeline_handle_t play_pipeline[AUDIO_NUM];

// audio_elements_handler 控制每個個別的音訊處理組成
// 含有 spiffs/mp3_decoder/i2s_stream_writer
static audio_element_handle_t spiffs_stream_reader[AUDIO_NUM];
static audio_element_handle_t audio_decoder[AUDIO_NUM];
static audio_element_handle_t i2s_stream_writer;

// 設置緩衝，用於設置 mp3_decoder 與 i2s_stream 間的緩衝
// 因 i2s_stream 只有一個，而 pipeline 有兩個，因此無法直接將一個 i2s_stream 加入兩個 pipeline
static ringbuf_handle_t ringbuf;

// esp_peripheral_handler，在此處理 spiffs
static esp_periph_set_handle_t set;

// audio_event_handle 處理音訊播放
static audio_event_iface_handle_t evt;

// audio_event_message 處理音訊狀態
static audio_event_iface_msg_t msg;

// 設置緩衝區大小
#define RINGBUF_INPUT_SIZE (6 * 1024)

// 設置兩個 audio_pipeline 實現循環播放、確保不同音訊間連接順暢
#define AUDIO_INDEX_ITERATE(index) for (int index = 0; index < AUDIO_NUM; index++)
typedef enum {
    AUDIO_FIRST = 0,
    AUDIO_SECOND,
    AUDIO_NUM
} audio_index_t;

int cur_index = 0;

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

    // 初始化 audio_elements, audio_pipeline
    AUDIO_INDEX_ITERATE(audio_index) {
        audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
        play_pipeline[audio_index] = audio_pipeline_init(&pipeline_cfg);
        mem_assert(play_pipeline[audio_index]);

        spiffs_stream_cfg_t flash_cfg = SPIFFS_STREAM_CFG_DEFAULT();
        flash_cfg.type = AUDIO_STREAM_READER;
        spiffs_stream_reader[audio_index] = spiffs_stream_init(&flash_cfg);

        mp3_decoder_cfg_t mp3_cfg = DEFAULT_MP3_DECODER_CONFIG();
        audio_decoder[audio_index] = mp3_decoder_init(&mp3_cfg);

        audio_pipeline_register(play_pipeline[audio_index], spiffs_stream_reader[audio_index], "file");
        audio_pipeline_register(play_pipeline[audio_index], audio_decoder[audio_index], "mp3");
        const char *link_tag[2] = {"file", "mp3"};
        audio_pipeline_link(play_pipeline[audio_index], &link_tag[0], 2);
    }

    // 初始化 i2s_stream
    i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT();
    i2s_cfg.type = AUDIO_STREAM_WRITER;
    i2s_cfg.use_alc = true;
    // 設置初始音量大小
    i2s_cfg.volume = 0;
    i2s_stream_writer = i2s_stream_init(&i2s_cfg);

    // 初始化音訊播放處理系統
    audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    evt = audio_event_iface_init(&evt_cfg);

    // set_listner for pipeline
    AUDIO_INDEX_ITERATE(audio_index) {
        audio_pipeline_set_listener(play_pipeline[audio_index], evt);
    }
    audio_element_msg_set_listener(i2s_stream_writer, evt);
    audio_event_iface_set_listener(esp_periph_set_get_event_iface(set), evt);

    // 初始化緩衝區大小
    ringbuf = rb_create(RINGBUF_INPUT_SIZE, 1);
}

// 設置 mp3 文件，選擇音檔
void set_mp3(const char *file_path) {
    AUDIO_INDEX_ITERATE(audio_index) {
    audio_pipeline_stop(play_pipeline[audio_index]);
    audio_pipeline_wait_for_stop(play_pipeline[audio_index]);
    audio_pipeline_reset_ringbuffer(play_pipeline[audio_index]);
    audio_pipeline_reset_elements(play_pipeline[audio_index]);
    audio_pipeline_reset_items_state(play_pipeline[audio_index]);
    audio_element_set_uri(spiffs_stream_reader[audio_index], file_path);
    audio_element_set_output_ringbuf(audio_decoder[audio_index], ringbuf);
    }
    audio_element_set_input_ringbuf(i2s_stream_writer, ringbuf);
}

// 啟動 mp3 播放功能
void start_mp3() {
    audio_element_run(i2s_stream_writer);
    audio_element_resume(i2s_stream_writer, 0, 2000 / portTICK_PERIOD_MS);
}

// 播放 mp3
void play_mp3() {
    audio_pipeline_run(play_pipeline[cur_index]);
}

// 處理音訊播放
void handle_audio_events()
{
    esp_err_t ret = audio_event_iface_listen(evt, &msg, portMAX_DELAY);

    if (ret != ESP_OK) {
        return;
    }

    if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.cmd == AEL_MSG_CMD_REPORT_MUSIC_INFO) {
        audio_element_info_t music_info = {0};
        static audio_element_info_t prev_music_info = {0};
        AUDIO_INDEX_ITERATE(audio_index) {
            if (msg.source != (void *) audio_decoder[audio_index]) {
                continue;
            }
            audio_element_getinfo(audio_decoder[audio_index], &music_info);
            if ((prev_music_info.bits != music_info.bits) || (prev_music_info.sample_rates != music_info.sample_rates)
                || (prev_music_info.channels != music_info.channels)) {
                i2s_stream_set_clk(i2s_stream_writer, music_info.sample_rates, music_info.bits, music_info.channels);
                memcpy(&prev_music_info, &music_info, sizeof(audio_element_info_t));
                cur_index = audio_index; 
            }
            break;
        }
        return;
    }

    if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.cmd == AEL_MSG_CMD_REPORT_STATUS &&
        ((int)msg.data == AEL_STATUS_STATE_FINISHED)) {
        replay_audio();
    }
}

// 重新播放音檔
void replay_audio() {
    AUDIO_INDEX_ITERATE(audio_index) {
        if (msg.source == (void *) audio_decoder[audio_index]) {
            int next_index = audio_index;
            if (audio_index == (AUDIO_NUM - 1)) {
                next_index = 0;
            } else {
                next_index++;
            }
            rb_reset_is_done_write(ringbuf);
            audio_element_set_output_ringbuf(audio_decoder[audio_index], NULL);
            audio_element_set_output_ringbuf(audio_decoder[next_index], ringbuf);
            audio_pipeline_run(play_pipeline[next_index]);

            audio_pipeline_stop(play_pipeline[audio_index]);
            audio_pipeline_wait_for_stop(play_pipeline[audio_index]);
            audio_pipeline_reset_ringbuffer(play_pipeline[audio_index]);
            audio_pipeline_reset_elements(play_pipeline[audio_index]);
            audio_pipeline_reset_items_state(play_pipeline[audio_index]);
            break;
        }
    }
}

// 暫停播放
void pause_audio() {
    AUDIO_INDEX_ITERATE(audio_index) {
        audio_pipeline_pause(play_pipeline[audio_index]);
    }
    audio_element_pause(i2s_stream_writer);
}

// 恢復播放
void resume_audio() {
    AUDIO_INDEX_ITERATE(audio_index) {
        audio_pipeline_resume(play_pipeline[audio_index]);
    }
    audio_element_resume(i2s_stream_writer, 0, portMAX_DELAY);
}

// 停止播放並銷毀
void stop_audio() {
    AUDIO_INDEX_ITERATE(audio_index) {
        audio_pipeline_stop(play_pipeline[audio_index]);
        audio_pipeline_wait_for_stop(play_pipeline[audio_index]);
        audio_pipeline_terminate(play_pipeline[audio_index]);
        audio_pipeline_unregister(play_pipeline[audio_index], spiffs_stream_reader[audio_index]);
        audio_pipeline_unregister(play_pipeline[audio_index], audio_decoder[audio_index]);
        audio_pipeline_remove_listener(play_pipeline[audio_index]);
    }
    audio_element_deinit(i2s_stream_writer);
    audio_event_iface_destroy(evt);
    rb_destroy(ringbuf);
}

// 設置音量大小
// volume 範圍有待商榷
void set_volume(int volume)
{
    i2s_alc_volume_set(i2s_stream_writer, volume);
}

