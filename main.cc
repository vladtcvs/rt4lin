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



int g_end = 0;


extern int ammo;


void draw_scene()
{
	clear_block(0,0,WS,HS);
	std::list<ship*>::iterator it, it2;
	for (it = ships.begin(); it != ships.end(); ++it)
	{
		(*it)->draw();	
	}
	char StatusString[1000] = {0};

	SDL_Color fColor ; // Font color (R,G,B)
	fColor.r = fColor.g = fColor.b = 245;

	clear_block(0,HS,WS,40);
	draw_line(0,HS+1,WS,HS, 0xFFFF0000);
	sprintf(StatusString, "Mode: %i Count: %i Lifes: %i", mode, count, you->mr);
	draw_text(StatusString, fColor, font, 5,HS+5);	

	draw_walls();

	SDL_RenderPresent(renderer);
}

Uint32 draw_t(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;


	userevent.type = SDL_USEREVENT;
    	userevent.code = 0;
    	userevent.data1 = NULL;
    	userevent.data2 = NULL;

   	 event.type = SDL_USEREVENT;
    	event.user = userevent;

    	SDL_PushEvent(&event);

	return interval;
}

Uint32 timer(Uint32 interval, void *param)
{

	wall_new();

	std::list<ship*>::iterator it, it2;
	x_0 += vx_0;
	y_0 += vy_0;
	pos0 -= vx_0;

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
	return interval;
}



bool left, right, up, down;










void normal_mode(SDL_Event event)
{
	if (event.key.repeat)
		return;

	switch (event.type) 
	{
	      	case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
		      	{
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
				case SDLK_LSHIFT:
					you->fire();
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
					you->vx = vx_0;
					break;

				case SDLK_RIGHT:
					right = false;
					you->vx = vx_0;
					break;
				case SDLK_UP:
					up = false;
					you->vy = vy_0;
					break;

				case SDLK_DOWN:
					down = false;
					you->vy = vy_0;
					break;
	
			}
		default:
			break;
	}
	

}




static int menu_item = 0;

void menu_mode(SDL_Event event)
{
	switch (event.type) 
	{
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
						{
							cur_menu = cur_menu->submenu[menu_item];
							menu_item = 0;
						}
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
				default:
					break;
			}
			break;
		default:
			break;
	}
	clear_block(0,0,WS,HS);
	cur_menu->draw(menu_item);
}


int end_game(void*)
{
	g_end = 1;
	return 0;
}






int main(void)
{
	SDL_Event event;


	srand(time(NULL));

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	
	SDL_Window *window = SDL_CreateWindow("RT4LIN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WS, HS+40, SDL_WINDOW_SHOWN);
	SDL_Renderer *rend = SDL_CreateRenderer(window, -1, 0);
	
	renderer = rend;

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

	SDL_TimerID enemy_timer_id = 0;
	SDL_TimerID my_timer_id = 0;
	SDL_TimerID draw_timer_id = 0;
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	SDL_RenderPresent(renderer);
	int rel = 1;	

	while (!g_end)
	{	
		int ns = ships.size();
		int i;
		char StatusString[1000]={0};


		while (!SDL_PollEvent(&event)){}
				
				
		
		
		switch(event.type)
		{
	      		case SDL_QUIT:
				g_end = 1;
		      		break;
		
			case SDL_USEREVENT:
				draw_scene();
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					mode = 1-mode;

					if (mode == 1)
					{
						cur_menu = &root_menu;
						menu_item = 0;
						if (my_timer_id)
						{
							SDL_RemoveTimer(my_timer_id);
							my_timer_id = 0;
						}
						if (draw_timer_id)
						{
							SDL_RemoveTimer(draw_timer_id);
							draw_timer_id = 0;
						}
						if (enemy_timer_id)
						{
							SDL_RemoveTimer(enemy_timer_id);
							enemy_timer_id = 0;
						}

					}
					else
					{
						my_timer_id = SDL_AddTimer(50, timer, NULL);
						draw_timer_id = SDL_AddTimer(1000/60, draw_t, NULL);
						enemy_timer_id = SDL_AddTimer(2000, add_en, NULL);
					}
					
				}
				rel = 0;
				break;
			case SDL_KEYUP:
				rel = 1;
				break;
			default:
				break;
		}
			
		if (mode == 1)
		{
			menu_mode(event);
		}	
		else
		{
			normal_mode(event);
		}
		
		
		SDL_RenderPresent(renderer);

		usleep(1e4);

	}
	ships.clear();
	free_menus();
	printf("Count: %i\n", count);
	SDL_Quit();
	return 0;
}


