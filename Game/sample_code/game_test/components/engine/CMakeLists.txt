set(srcs "src/Engine.c" "src/Resource.c" "src/Render.c" "src/Joystick.c" "src/Keyboard.c" "src/Audio.c")
set(requires st7789 fontx pngle decode_png keyboardlib esp_adc driver my_board speaker)

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}")   