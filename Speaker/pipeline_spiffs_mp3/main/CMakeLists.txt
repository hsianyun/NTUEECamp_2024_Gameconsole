set(COMPONENT_SRCS "play_spiffs_mp3_example.c")
set(COMPONENT_ADD_INCLUDEDIRS .)

register_component(my_component)

spiffs_create_partition_image(storage ../tools FLASH_IN_PROJECT)