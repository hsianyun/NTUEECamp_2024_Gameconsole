<<<<<<< HEAD
set(srcs "src/Engine.c" "src/Resource.c" "src/Render.c" "src/Joystick.c" "src/Keyboard.c")
set(requires st7789 fontx pngle decode_png keyboardlib esp_adc driver)
=======
set(srcs "src/ResourceManager.c" "src/RenderManager.c")
set(requires st7789 fontx pngle decode_png)
>>>>>>> 5cf654a1f1c36b7c9e50f5235b011ecb37ea1292

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}")   