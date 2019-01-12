CC = g++
CFLAGS = -Wall -Werror -Wshadow -pedantic -std=gnu++11
INCLUDE_FLAGS = -I/usr/include/SDL2/
LIBRARY_FLAGS = -lSDL2 -lGLEW
DEFINE_FLAGS = -DGLEW_STATIC

graphics:
	$(CC) main.cpp -o grun $(CFLAGS) $(DEFINE_FLAGS) $(INCLUDE_FLAGS) $(LIBRARY_FLAGS)
