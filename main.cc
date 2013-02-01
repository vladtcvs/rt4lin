#include <list>
#include <SDL.h>
#include <math.h>
#include <time.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <png.h>
#include <unistd.h>


#include "draw.h"
#include "data_structures.h"
#include "init.h"
#include "ship.h"

#define abs(x) ((x)<0?(-(x)):(x))
#define sqr(x) ((x)*(x))






int g_end = 0;


extern int ammo;



Uint32 timer(Uint32 interval, void *param)
{
	if (mode != 0)
		return interval;

	x_0 += vx_0;
	y_0 += vy_0;
	pos0 -= vx_0;

	std::list<ship*>::iterator it, it2;
	for (it = ships.begin(); it != ships.end(); ++it)
	{
			(*it)->step();	
			(*it)->activity();
	}


	for (it = ships.begin(); it != ships.end();++it)
	{
		/* проверка столкновения со стеной */
		
		double y = (*it)->y;
		double x = (*it)->x-(x_0+pos0);
		
		int k = x/(w/3);
		int j;
		
		double y1 = mountain[0][k];
		double y2 = mountain[0][k+1];
		double dx = (x-k*w/3.)/(w/3);
		double ym = y1*(1-dx)+y2*dx;
		if (y < ym+0.1)
		{
			(*it)->mr=0;
		}
		y1 = mountain[1][k];
		y2 = mountain[1][k+1];
		ym = y1*(1-dx)+y2*dx;
		if (y > ym-0.1)
		{
			(*it)->mr=0;
		}
			
			
		for (it2 = ships.begin(); it2 != ships.end();++it2)
		{
			if (it == it2)
				continue;
			
			if ((*it)->intersect(*it2))
			{
				(*it)->mr+=sides_inter[(*it)->side][(*it2)->side];
				if ((*it)->mr == 0)
				{
					if ((*it)->side == 1)
						count++;
				}
			}
		}
	}



	for (it = ships.begin(); it != ships.end();)
	{
		if ((*it)->x < x_0 || (*it)->x > x_0 + w || (*it)->y < 0 || (*it)->y > h || (*it)->mr <= 0)
		{
			if (*it == you)
			{
				printf("Game over\n");
				// проиграли!!
				g_end = 1;
			}
			if ((*it)->mr <= 0)
				(*it)->on_die();
			it = ships.erase(it);
		}
		else
		{
			++it;
		}
	}



	return interval;
}

double y_rand()
{
	double x = w-0.2-pos0;
	int k1, k2;
	k1 = x/(w/3);
	k2 = k1+1;
	double x1 = k1*w/3;
	double x2 = k2*w/3;
	double y1_1 = mountain[0][k1];
	double y2_1 = mountain[0][k2];

	double y1_2 = mountain[1][k1];
	double y2_2 = mountain[1][k2];
	
	double dx = (x-x1)/(w/3);
	double yh = y1_1*dx + y1_2*(1-dx);
	double yl = y2_1*dx + y2_2*(1-dx);

	int p = 20+rand()%60;
	double y = yl + p/99.*(yh-yl);
	return y;
}


Uint32 add_en(Uint32 interval, void *param)
{
	if (mode == 0)
	{
		double y = y_rand();
		ship *en = new enemy;
		en->x = x_0+w-0.2;
		en->y = y;
		en->vx = -0.02;
		en->mr=1;
		en->vy = 0;
		en->alpha=180;
		en->side = O_ENEM;
		ships.push_back(en);


		if (rand()%40 == 0)
		{
			y = y_rand();
			ship *lb = new life_bonus;
			lb->x = x_0+w-0.2;
			lb->y = y;
			lb->alpha=180;
			lb->side = O_LBONUS;
			ships.push_back(lb);
		}
	}
	return interval;
}



bool left, right, up, down;






void wall_new()
{
	if (pos0 < -w/3)
	{
		int i;
		for (i = 0; i <= 3; i++)
		{
			mountain[0][i]=mountain[0][i+1];
			mountain[1][i]=mountain[1][i+1];
		}
		mountain[0][4] = (rand()%(HS/4))*h/HS;
		mountain[1][4] = (rand()%(HS/4))*h/HS+h*3/4.;
		pos0 = 0;
	}
}



void normal_mode(SDL_Surface *screen, SDL_Event event)
{
	std::list<ship*>::iterator it, it2;
	for (it = ships.begin(); it != ships.end(); ++it)
	{
		(*it)->draw(screen);	
	}
			
	
	draw_walls(screen);
	wall_new();
	if (SDL_PollEvent(&event)) 
	{
		switch (event.type) 
	      	{
		      	case SDL_QUIT:
				g_end = 1;
			      	break;
		      	case SDL_KEYDOWN:
		      		switch(event.key.keysym.sym)
		      		{
					case SDLK_ESCAPE:
						cur_menu = &root_menu;
						mode = 1;
						break;
		      			case SDLK_LEFT:
						left = true;
						if (lsp)
							you->vx = vx_0-0.025;
						else 
							you->vx = vx_0-0.05;
				        	break;
					case SDLK_RIGHT:
						right = true;
						you->vx = vx_0+0.05;
						break;
		      			case SDLK_UP:
						up = true;
						you->vy = vy_0-0.05;
				        	break;
					case SDLK_DOWN:
						down = true;
						you->vy = vy_0+0.05;
						break;
					default:
						break;
				}
				break;
			case SDL_KEYUP:
				switch(event.key.keysym.sym)
		      		{

		      			case SDLK_LEFT:
						left = false;
						if (right == false)
						you->vx = vx_0;
				        	break;
					case SDLK_RIGHT:
						right = false;
						if (left == false)
							you->vx = vx_0;
						break;
		      			case SDLK_UP:
						up = false;
						if (down == false)
							you->vy = vy_0;
				        	break;
					case SDLK_DOWN:
						down = false;
						if (up == false)
							you->vy = vy_0;
						break;
					case SDLK_LCTRL:
						you->fire();
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}

}




void menu_mode(SDL_Surface *screen, SDL_Event event)
{
	static int menu_item = 0;
	if (SDL_PollEvent(&event)) 
	{
		switch (event.type) 
	      	{
		      	case SDL_QUIT:
				g_end = 1;
			      	break;
		      	case SDL_KEYDOWN:
		      		switch(event.key.keysym.sym)
		      		{
		      			case SDLK_UP:
						if (menu_item > 0)				
							--menu_item;
				        	break;
					case SDLK_DOWN:
						if (menu_item < cur_menu->submenu.size()-1)
							++menu_item;
						break;
					case SDLK_RETURN:
						if (cur_menu->submenu[menu_item]->action == NULL)
						{
							if (menu_item < cur_menu->submenu.size())
								cur_menu = cur_menu->submenu[menu_item];
						}
						else 
						{
							int (*f)(void*) = cur_menu->submenu[menu_item]->action;
							f(cur_menu->submenu[menu_item]->value);
						}
						break;
					case SDLK_PAGEUP:
						if (cur_menu->parent != NULL)
							cur_menu = cur_menu->parent;
						break;
					case SDLK_ESCAPE:
						mode = 0;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
	cur_menu->draw(screen, menu_item);
}


int end_game(void*)
{
	g_end = 1;
	return 0;
}






int main(void)
{
	SDL_Event event;

	SDL_Surface *imgTxt ;
	SDL_Rect txtRect ; // Store (x,y) of text for blit
	SDL_Color fColor ; // Font color (R,G,B)
	txtRect.x = 10;
	txtRect.y = HS+5;
	fColor.r = fColor.g = fColor.b = 245;

	srand(time(NULL));

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);

	SDL_WM_SetCaption("RTYPE LINUX", "RTYPE LINUX");
	SDL_Surface* screen = SDL_SetVideoMode(WS, HS+40, 32, SDL_DOUBLEBUF|SDL_HWSURFACE);
	
	init_fonts();
	init_wall();
	init_inter();
	load_images();
	init_menus();

	x_0 = 0;
	y_0 = 0;
	
	ship *ns;
	ships.push_back(you=new character);

	you->x = 1;
	you->side = 0;
	you->y = 2;
	you->alpha = 0;
	you->vx = vx_0;
	you->vy = vy_0;
	
	mode = 1;

	SDL_TimerID enemy_timer_id = SDL_AddTimer(2000, add_en, NULL);
	SDL_TimerID my_timer_id = SDL_AddTimer(50, timer, NULL);

	
	while (!g_end)
	{	
		int ns = ships.size();
		int i;
		char StatusString[1000]={0};
		SDL_FillRect(screen, NULL, 0x000000);


		my_draw_line(screen, 0, HS-1, WS, HS-1, 0x00FF00FF);		

		sprintf(StatusString, "Count: %i   Ammo: %i    Objects: %i    Angry: %i    Lifes: %i", count, 
						ammo, (int)ships.size(), count%20, you->mr);


		imgTxt = TTF_RenderText_Solid(font, StatusString, fColor);
		SDL_BlitSurface(imgTxt, NULL, screen, &txtRect );
	
		if (mode)
			menu_mode(screen, event);
		else
			normal_mode(screen, event);
		usleep(1e6/50);

		SDL_Flip(screen);
	}
	SDL_RemoveTimer(my_timer_id);
	SDL_RemoveTimer(enemy_timer_id);
	ships.clear();
	free_menus();
	printf("Count: %i\n", count);
	SDL_Quit();
	return 0;
}


