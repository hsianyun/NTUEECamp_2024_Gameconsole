set(srcs "src/Game.c")
set(requires engine person bullet files treasure weapon cpu)
set(priv_requires st7789 fontx pngle decode_png keyboardlib esp_adc driver my_board speaker engine)

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "include"
                       REQUIRES "${requires}"
                       PRIV_REQUIRES "${priv_requires}")