idf_component_register(SRCS "src/my_component.c"
                    INCLUDE_DIRS "include"
                    REQUIRES audio_pipeline audio_stream audio_hal esp_peripherals)  