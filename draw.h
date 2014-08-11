#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H

#include <SDL.h>
#include <SDL_ttf.h>

typedef SDL_Texture Texture;

void draw_object(Texture *image, double x, double y);
void draw_text(char *str, SDL_Color fcolor, TTF_Font *font, int x, int y);
void clear_block(int x, int y, int bw, int bh);

Texture *load_image(const char *filename);

void load_images();
void draw_walls();
void draw_line(int x1, int y1, int x2, int y2, Uint32 color);


#endif
