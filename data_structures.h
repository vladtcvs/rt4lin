#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H


#include <SDL.h>
#include <list>
#include "ship.h"
#include "menu.h"
#include <SDL_ttf.h>

extern SDL_Surface *char_image;
extern SDL_Surface *rocket_image;
extern SDL_Surface *enemy_image;
extern SDL_Surface *enemy_bomb_image;
extern SDL_Surface *trees[10];
extern int ntrees;

extern bool sides_inter[10][10];

extern character *you;
extern std::list<ship*> ships;


extern double mountain[2][10];
extern double pos0;

extern double x_0, y_0, w, h;
extern double vx_0, vy_0;


extern int WS, HS;

extern menu root_menu;
extern menu *cur_menu;

extern int count;
extern TTF_Font *font;

extern int mode, g_mode;

#define O_CHAR 0
#define O_ENEM 1
#define O_BOMB 2
#define O_WALL 3





#endif
