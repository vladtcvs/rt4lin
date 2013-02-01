#ifndef MENU_H
#define MENU_H

#include <vector>
#include <SDL.h>

struct menu
{
	char name[100];
	int *value;
	menu *parent;
	std::vector<menu*> submenu;
	void draw(SDL_Surface *screen, int menu_item);
	int (*action)(void *);
	menu();
};


int ch_value(void*pn);
int ch_mode(void*);


#endif
