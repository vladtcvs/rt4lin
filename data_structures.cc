#include <SDL.h>
#include <list>
#include "data_structures.h"


SDL_Texture *char_image;
SDL_Texture *rocket_image;
SDL_Texture *enemy_image;
SDL_Texture *enemy_bomb_image;
SDL_Texture *life_bonus_image;
SDL_Texture *trees[10];


int ntrees;


SDL_Renderer *renderer;

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
