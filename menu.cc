#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include "menu.h"
#include "data_structures.h"

void menu::draw(SDL_Surface *screen, int menu_item)
{
	SDL_Surface *text;
	SDL_Rect txtRect;
	SDL_Color fColor;
	fColor.r = fColor.g = fColor.b = 245;

	txtRect.x = 10;
	txtRect.y = 10;
	
	char str[100];
	int val = 0;
	if (value != NULL)
	{
		val = *value;
		sprintf(str, "%s : %i", name, val);
	}
	else
		sprintf(str, "%s", name);

	text = TTF_RenderText_Solid(font, str, fColor);
        SDL_BlitSurface(text, NULL, screen, &txtRect );
	
	int i, l = submenu.size();
	for (i = 0; i < l; i++)
	{
		txtRect.y = 40+40*i;
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
		text = TTF_RenderText_Solid(font, str, fColor);
	        SDL_BlitSurface(text, NULL, screen, &txtRect );
	}
	
}

menu::menu()
{
	action = NULL;
}



int ch_mode(void*)
{
	g_mode = 1-g_mode;
	return 0;
}
