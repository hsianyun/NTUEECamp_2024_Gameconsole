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
    initialize_audio_system();

    set_mp3("/spiffs/adf_music.mp3");
    start_mp3();

    play_mp3();
    
    while (1) {
        set_volume(-15);
        handle_audio_events();
    }

    stop_audio();
}

