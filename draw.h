#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H

#include <SDL.h>
#include "ship.h"

void draw_object(SDL_Surface *screen, SDL_Surface *image, double x, double y);
SDL_Surface *load_image(const char *filename);
void load_images();
void draw_walls(SDL_Surface *screen);
void my_draw_line(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color);


#endif
