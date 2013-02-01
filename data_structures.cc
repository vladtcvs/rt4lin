#include <SDL.h>
#include <list>
#include "data_structures.h"


SDL_Surface *char_image;
SDL_Surface *rocket_image;
SDL_Surface *enemy_image;
SDL_Surface *enemy_bomb_image;
SDL_Surface *life_bonus_image;
SDL_Surface *trees[10];
int ntrees;

int sides_inter[10][10];

character *you;
std::list<ship*> ships;


double mountain[2][10];
double pos0;


double x_0, y_0, w=8, h=4;
double vx_0 = 0.05, vy_0 = 0;


int WS =800, HS=400;


int count = 0;

TTF_Font *font;


menu root_menu;
menu *cur_menu;


int eexpl=0, horiz=0, lsp=0;

int mode = 0, g_mode = 0;
