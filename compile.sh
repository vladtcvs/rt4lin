#!/bin/sh

clang++ -lSDL -lSDL_ttf -lSDL_image -I/usr/include/SDL -o rtype2 main.cc libSDL_draw.a -lpng
