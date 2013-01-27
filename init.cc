#include "data_structures.h"
#include <SDL_ttf.h>

#define SET_INTER(a,b,v) {sides_inter[a][b] = v; sides_inter[b][a] = v;}

void init_inter()
{
	SET_INTER(O_CHAR, O_ENEM, true)
	SET_INTER(O_CHAR, O_BOMB, true)
	SET_INTER(O_CHAR, O_WALL, true)
	SET_INTER(O_ENEM, O_WALL, true)
	SET_INTER(O_BOMB, O_WALL, true)
}



void init_wall()
{
	for (int i = 0; i <= 4; i++)
	{
		mountain[0][i] = (rand()%(HS/4))*h/HS;
		mountain[1][i] = (rand()%(HS/4))*h/HS+h*3/4.;
	}
}

void init_fonts()
{
	TTF_Init();
	font = TTF_OpenFont("./font.ttf",20);

}


int end_game(void*);

void init_menus()
{
	strcpy(root_menu.name, "MENU");
	root_menu.value = NULL;

	menu* chmode;

	chmode = new menu;
	chmode->action = ch_mode;
	chmode->value = &g_mode;
	strcpy(chmode->name, "Classic/modern");
	chmode->parent = &root_menu;
	root_menu.submenu.push_back(chmode);

	chmode = new menu;
	chmode->action = end_game;
	chmode->value = NULL;
	strcpy(chmode->name, "Quit");
	chmode->parent = &root_menu;
	root_menu.submenu.push_back(chmode);

	cur_menu = &root_menu;
}


void free_menus()
{
	
}
