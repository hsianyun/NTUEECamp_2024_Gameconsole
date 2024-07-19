set(srcs "src/Cpu.c")
set(requires game)

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "include"
                       REQUIRES "${requires}")

