#include <stdio.h>
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
#include "my_component.h"

void app_main(void) {
    // 初始化音頻系統
    initialize_audio_system();

    // 設置播放文件
    set_mp3("/spiffs/adf_music.mp3");
    start_mp3();
    play_mp3();
    
    while (1) {
        // 控制音量
        set_volume(-15);
        // 使之重複播放
        set_audio_loop(true);
        // 監測播放狀態
        handle_audio_events();
    }

    // 停止並銷毀
    terminate_audio();
}

