idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES game)

spiffs_create_partition_image(storage ../tools FLASH_IN_PROJECT)
