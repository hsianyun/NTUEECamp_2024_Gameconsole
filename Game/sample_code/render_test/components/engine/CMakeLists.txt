set(srcs "src/ResourceManager.c" "src/RenderManager.c")
set(requires st7789 fontx pngle decode_png)

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}")   