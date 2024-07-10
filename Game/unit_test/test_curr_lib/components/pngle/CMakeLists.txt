set(srcs "src/pngle.c" )

idf_component_register(SRCS "${srcs}"
                       PRIV_REQUIRES driver
                       INCLUDE_DIRS "include")
