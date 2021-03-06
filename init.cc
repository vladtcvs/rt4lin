#include "data_structures.h"
#include <SDL_ttf.h>
#include <stdlib.h>
#define SET_INTER(a,b,v) {sides_inter[a][b] = v; sides_inter[b][a] = v;}
#define SET_INTER_A(a,b,v) {sides_inter[a][b] = v;}

#include "wall.h"

void init_inter()
{
	SET_INTER(O_CHAR, O_ENEM, -1)
	SET_INTER(O_YBOMB, O_ENEM, -1)
	SET_INTER(O_CHAR, O_BOMB, -1)
	SET_INTER(O_CHAR, O_WALL, -1)
	SET_INTER(O_ENEM, O_WALL, -1)
	SET_INTER(O_BOMB, O_WALL, -1)
	SET_INTER(O_BOMB, O_YBOMB, -1)
	SET_INTER_A(O_LBONUS, O_CHAR, -1)
	SET_INTER_A(O_CHAR, O_LBONUS, 1)
}



void init_wall()
{
	for (int i = 0; i <= 10; i++)
	{
		pos0 = -w;
		wall_new();
	}
}

void init_fonts()
{
	TTF_Init();
	font = TTF_OpenFont("./font.ttf",20);
	if (font == NULL)
	{
		printf("Loading font failed!\n");
		exit(0);
	}
}


int end_game(void*);

void init_menus()
{
	strcpy(root_menu.name, "MENU");
	root_menu.value = NULL;

	menu* chmode;

	chmode = new menu;
	chmode->action = ch_mode;
	chmode->value = NULL;
	strcpy(chmode->name, "Set classic");
	chmode->parent = &root_menu;
	root_menu.submenu.push_back(chmode);

	chmode = new menu;
	chmode->action = end_game;
	chmode->value = NULL;
	strcpy(chmode->name, "Quit");
	chmode->parent = &root_menu;
	root_menu.submenu.push_back(chmode);

	chmode = new menu;
	chmode->action = NULL;
	chmode->value = NULL;
	strcpy(chmode->name, "Options");
	chmode->parent = &root_menu;
	root_menu.submenu.push_back(chmode);

	menu *options = chmode;
	
	chmode = new menu;
	chmode->action = ch_value;
	chmode->value = &horiz;
	strcpy(chmode->name, "Fire horizontally");
	chmode->parent = options;
	options->submenu.push_back(chmode);
	
	chmode = new menu;
	chmode->action = ch_value;
	chmode->value = &eexpl;
	strcpy(chmode->name, "Explosion of enemy ships");
	chmode->parent = options;
	options->submenu.push_back(chmode);

	chmode = new menu;
	chmode->action = ch_value;
	chmode->value = &lsp;
	strcpy(chmode->name, "Low back speed");
	chmode->parent = options;
	options->submenu.push_back(chmode);

	chmode = new menu;
	chmode->action = ch_value;
	chmode->value = &autom;
	strcpy(chmode->name, "Automobile mode");
	chmode->parent = options;
	options->submenu.push_back(chmode);



	cur_menu = &root_menu;
}


void free_menu(menu *m)
{
	std::vector<menu*>::iterator it;
	for (it = m->submenu.begin(); it != m->submenu.end(); ++it)
	{
		menu *mm = *it;
		free_menu(mm);
		delete mm;
	}
	m->submenu.clear();
}

void free_menus()
{
	free_menu(&root_menu);
}

