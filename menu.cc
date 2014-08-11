#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include "menu.h"
#include "data_structures.h"

void menu::draw(int menu_item)
{
	SDL_Color fColor;
	fColor.r = fColor.g = fColor.b = 245;


	char str[100];
	int val = 0;
	if (value != NULL)
	{
		val = *value;
		sprintf(str, "%s : %i", name, val);
	}
	else
		sprintf(str, "%s", name);

	draw_text(str, fColor, font, 10, 10);


	int i, l = submenu.size();
	for (i = 0; i < l; i++)
	{
		val = 0;
		if (submenu[i]->value != NULL)
		{
			val = *(submenu[i]->value);
			sprintf(str, "%s  %s : %i", (i==menu_item)?"*":" ", submenu[i]->name, val);
		}
		else
		{
			sprintf(str, "%s  %s", (i==menu_item)?"*":" ", submenu[i]->name);
		}
		draw_text(str, fColor, font, 10, 40+40*i);

	}
	
}

menu::menu()
{
	action = NULL;
}


int ch_mode(void*)
{
	lsp = 0;
	eexpl = 0;
	horiz = 1;
	return 0;
}

int ch_value(void* pn)
{
	int *p = (int*)pn;
	*p = 1-*p;
	return 0;
}


