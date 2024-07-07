set(srcs "src/st7789.c" )
set(requires "fontx" "pngle" "decode_png" driver)

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "include"
                       REQUIRES "${requires}")
