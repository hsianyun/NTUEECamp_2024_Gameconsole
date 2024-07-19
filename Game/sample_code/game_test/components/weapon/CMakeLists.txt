set(srcs "src/Weapon.c")
set(requires engine person)
set(priv_requires st7789 fontx pngle decode_png keyboardlib esp_adc driver my_board speaker)

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "include"
                       REQUIRES "${requires}"
                       PRIV_REQUIRES "${priv_requires}")

