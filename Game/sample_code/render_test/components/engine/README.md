## Simple Game Engine

### Description

This is a game engine designed for NTUEE game console.

### Architecture

```
└──engine/
   ├──include/
   │  ├──Engine.h
<<<<<<< HEAD
   │  ├──Joystick.h
   │  ├──Keyboard.h
   │  ├──Resource.h
   │  └──Render.h
   ├──src/
   │  ├──Engine.c
   │  ├──Joystick.c
   │  ├──Keyboard.c
   │  ├──Resource.c
   │  └──Render.c
=======
   │  ├──ResourceManager.h
   │  └──RenderManager.h
   ├──src/
   │  ├──ResourceManager.c
   │  └──RenderManager.c
>>>>>>> 5cf654a1f1c36b7c9e50f5235b011ecb37ea1292
   ├──CMakeLists.txt
   └──README.md
```

### How to use?

Include following file in `main.c`
```C
#include "Engine.h"
```
For more usage of the engine, you can visit the [tutorial](https://hackmd.io/@uzF57KwOT5SnkVGD3Ppheg/Bkq8tLnLC) here.

### Sample code

[render_test](https://drive.google.com/file/d/1fSTIGNiYqWQUzMxRMaV6cDyT94TVNQgK/view?usp=sharing) (After building and flashing, you should see 5 people moving on the screen.)
