#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H


#include <SDL.h>
#include <list>
#include "ship.h"
#include "menu.h"
#include <SDL_ttf.h>

extern SDL_Texture *char_image;
extern SDL_Texture *rocket_image;
extern SDL_Texture *enemy_image;
extern SDL_Texture *enemy_bomb_image;
extern SDL_Texture *life_bonus_image;
extern SDL_Texture *trees[10];
extern int ntrees;

extern SDL_Renderer *renderer;
extern int sides_inter[10][10];

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

extern int mode;
extern int eexpl, horiz, lsp, autom;

#define O_CHAR 0
#define O_ENEM 1
#define O_BOMB 2
#define O_WALL 3
#define O_YBOMB 4
#define O_LBONUS 5





#endif
