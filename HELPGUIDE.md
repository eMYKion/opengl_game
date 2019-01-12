# Helpguide

These topics are in no particular order

## Installing SDL2

Stackoverflow: [https://askubuntu.com/questions/707001/ubuntu-15-10-installing-sdl2](https://askubuntu.com/questions/707001/ubuntu-15-10-installing-sdl2)

(just libraries)
`$ sudo apt-get install libsdl2-2.0`


(development)
`$ sudo apt-get install libsdl2-dev`

## Installing GLEW

(development)
`$ sudo apt-get install libglew-dev`

## Including SDL2

Include `/usr/include/SDL2` instead of `usr/include/SDL`.

In Makefile, use flag: `-I/usr/include/SDL2`

## Including GLEW

???

## Linking SDL2, GLEW

use compilation flags `-lSDL2 -lGLEW`

## getting SEGFAULT at `glGenBuffers()`

According to [OpenGL forums](https://www.opengl.org/discussion_boards/showthread.php/170104-glGenBuffers-segfault),

call glGenBuffers after window and context creation (with SDL).
