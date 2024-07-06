set(srcs "src/fontx.c" )

idf_component_register(SRCS "${srcs}"
                       PRIV_REQUIRES driver
                       INCLUDE_DIRS "include")
