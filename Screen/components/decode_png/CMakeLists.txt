set(srcs "src/decode_png.c" )
set(requires "pngle" )

idf_component_register(SRCS "${srcs}"
                       PRIV_REQUIRES driver
                       INCLUDE_DIRS "include"
                       REQUIRES "${requires}")
