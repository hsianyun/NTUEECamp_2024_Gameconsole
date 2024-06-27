#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_common.h"
//#include "board.h"
#include "esp_peripherals.h"
#include "periph_spiffs.h"
#include "spiffs_stream.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"
#include "string.h"
#include "driver/i2s_std.h"

// static const char *TAG = "PLAY_FLASH_MP3_CONTROL";

audio_element_handle_t spiffs_stream_reader, i2s_stream_writer, audio_decoder;

static audio_pipeline_handle_t play_pipeline;

void app_main(void)
{
    audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
    play_pipeline = audio_pipeline_init(&pipeline_cfg);
    mem_assert(play_pipeline);

    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
    esp_periph_set_handle_t set = esp_periph_set_init(&periph_cfg);

    periph_spiffs_cfg_t spiffs_cfg = {
        .root = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };
    esp_periph_handle_t spiffs_handle = periph_spiffs_init(&spiffs_cfg);

    // Start spiffs
    esp_periph_start(set, spiffs_handle);

    // Wait until spiffs is mounted
    while (!periph_spiffs_is_mounted(spiffs_handle)) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    //ESP_LOGI(TAG, "[Playing] Create fatfs stream to read data from sdcard");
    spiffs_stream_cfg_t flash_cfg = SPIFFS_STREAM_CFG_DEFAULT();
    flash_cfg.type = AUDIO_STREAM_READER;
    spiffs_stream_reader = spiffs_stream_init(&flash_cfg);

    //ESP_LOGI(TAG, "[Playing]  Create i2s stream to write audio data to MAX98357A");
    i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT();
    i2s_cfg.type = AUDIO_STREAM_WRITER;
    i2s_cfg.use_alc = true;
    i2s_cfg.volume=-10;
	i2s_stream_writer = i2s_stream_init(&i2s_cfg);

    mp3_decoder_cfg_t mp3_cfg = DEFAULT_MP3_DECODER_CONFIG();
    audio_decoder = mp3_decoder_init(&mp3_cfg);
    
	audio_pipeline_register(play_pipeline, spiffs_stream_reader, "file");
    audio_pipeline_register(play_pipeline, audio_decoder, "mp3");
    audio_pipeline_register(play_pipeline, i2s_stream_writer, "i2s");

    const char *link_tag[3] = {"file", "mp3", "i2s"};
    audio_pipeline_link(play_pipeline, &link_tag[0], 3);
    
    audio_element_set_uri(spiffs_stream_reader, "/spiffs/Test.mp3");
    
	audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    audio_event_iface_handle_t evt = audio_event_iface_init(&evt_cfg);

	audio_pipeline_set_listener(play_pipeline, evt);

    audio_pipeline_run(play_pipeline);
   
    while (1) {
        audio_event_iface_msg_t msg;
        esp_err_t ret = audio_event_iface_listen(evt, &msg, portMAX_DELAY);

        if (ret != ESP_OK) {
            continue;
        }
		
        if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT
            && msg.source == (void *) audio_decoder
            && msg.cmd == AEL_MSG_CMD_REPORT_MUSIC_INFO) {
                audio_element_info_t music_info = {0};
            audio_element_getinfo(audio_decoder, &music_info);
            audio_element_setinfo(i2s_stream_writer, &music_info);
            i2s_stream_set_clk(i2s_stream_writer, music_info.sample_rates, music_info.bits, music_info.channels);
            continue;
        }
	
        if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT
            && (msg.source == (void *) i2s_stream_writer || msg.source == (void *) spiffs_stream_reader)
            && msg.cmd == AEL_MSG_CMD_REPORT_STATUS && 
			(((int)msg.data == AEL_STATUS_STATE_STOPPED) || ((int)msg.data == AEL_STATUS_STATE_FINISHED))) {            
            break;
        }

        
    }
    ////ESP_LOGI(TAG, "[Playing]  Stop audio_pipeline");
    audio_pipeline_stop(play_pipeline);
    audio_pipeline_wait_for_stop(play_pipeline);
    audio_pipeline_terminate(play_pipeline);

	audio_pipeline_unregister(play_pipeline, spiffs_stream_reader);
    audio_pipeline_unregister(play_pipeline, audio_decoder);
    audio_pipeline_unregister(play_pipeline, i2s_stream_writer);
   
    /* Terminal the pipeline before removing the listener */
    audio_pipeline_remove_listener(play_pipeline);

    /* Make sure audio_pipeline_remove_listener & audio_event_iface_remove_listener are called before destroying event_iface */
    audio_event_iface_destroy(evt);

    /* Release all resources */
    audio_pipeline_deinit(play_pipeline);
    audio_element_deinit(spiffs_stream_reader);
    audio_element_deinit(i2s_stream_writer);
    audio_element_deinit(audio_decoder);
	vTaskDelete(NULL);
}