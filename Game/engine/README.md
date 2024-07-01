## Simple Game Engine

### Description

This is a game engine designed for NTUEE game console.

### Architecture

```
└──engine/
   ├──include/
   │  ├──Engine.h
   │  ├──ResourceManager.h
   │  └──RenderManager.h
   ├──src/
   │  ├──ResourceManager.c
   │  └──RenderManager.c
   ├──CMakeLists.txt
   └──README.md
```

### How to use?

(Running on ESP32 has not been tested yet)
You can visit the [tutorial](https://hackmd.io/@uzF57KwOT5SnkVGD3Ppheg/Bkq8tLnLC) here, which is not completed yet.

### Develop

If you perfer developing without using ESP32, you can use following Structure

```
├──components/engine/
│  ├──include/
│  │  ├──Engine.h
│  │  ├──ResourceManager.h
│  │  └──RenderManager.h
│  └──src/
│     ├──ResourceManager.c
│     └──RenderManager.c
├──files/
│  └──data.h
└──main/
   └──main.c
```
Include your own datas in `data.h`. For example,
```C
//in data.h
#include <stdint.h>

uint8_t testData[] = 
{
    1,2,3,
    4,5,6,
    7,8,9
};
```
then include these header files in `main.c`
```C
#include "../components/engine/include/Engine.h"
#include "../files/data.h"
```
When compiling, run following command
```
gcc -Wall -Wextra -g3 [file_location]/main/main.c [file_location]/components/engine/src/ResourceManager.c [file_location]/components/engine/src/RenderManager.c  -I[file_location]/components/engine/include -o [file_location]/main/output/main
```
then run
```
./main
```
