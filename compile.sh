#!/bin/sh

CXX=clang++
$CXX -lSDL -lSDL_ttf -lSDL_image -I/usr/include/SDL -o rtype2 main.cc  -lpng

