## Simple Game Engine

### Description

This is a game engine designed for NTUEE game console.

### Architecture

```
└──engine/
   ├──include/
   │  ├──Engine.h
   │  ├──Joystick.h
   │  ├──Keyboard.h
   │  ├──Resource.h
   │  ├──Render.h
   │  └──Audio.h
   ├──src/
   │  ├──Engine.c
   │  ├──Joystick.c
   │  ├──Keyboard.c
   │  ├──Resource.c
   │  ├──Render.c
   │  └──Audio.c
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
