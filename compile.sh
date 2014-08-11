#!/bin/sh

CXX=clang++
$CXX -lSDL -lSDL_ttf -lSDL_image -I/usr/include/SDL2 -o rtype2 main.cc draw.cc data_structures.cc init.cc ship.cc character.cc enemy.cc passive.cc menu.cc life_bonus.cc -lpng

