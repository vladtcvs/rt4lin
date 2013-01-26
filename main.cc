#include <list>
#include <SDL.h>
#include "SDL_draw.h"
#include <math.h>
#include <time.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <png.h>
#include <unistd.h>

double x_0, y_0, w=8, h=4;
double vx_0 = 0.05, vy_0 = 0;


int WS =800, HS=400;

#define abs(x) ((x)<0?(-(x)):(x))


SDL_Surface *char_image;
SDL_Surface *rocket_image;
SDL_Surface *enemy_image;
SDL_Surface *enemy_bomb_image;


bool sides_inter[3][3] = 
{
{false, true, true},
{true, false, false},
{true, false, false}
};


int g_end = 0;

struct ship
{
	SDL_Surface *image;
	int mr;
	int side;
	double x, y;
	double alpha;
	double vx, vy;
	virtual void draw(SDL_Surface *screen);
	virtual void move(double dx, double dy);
	void step(void);
	virtual void chv(void){}
	virtual void activity(){}
	virtual void on_die(){}
	virtual void fire(void){}
	virtual bool intersect(ship *other);
	ship();
};



std::list<ship*> ships;

int count = 0;
int ammo = 0;


ship::ship()
{
	image = NULL;
	mr = 1;
}

void ship::step(void)
{
	x += vx;
	y += vy;
	chv();
}

void ship::move(double dx, double dy)
{
	x += dx;
	y += dy;
}

bool ship::intersect(ship *other)
{
	double l2 = (x-other->x)*(x-other->x)+(y-other->y)*(y-other->y);
	return (l2 < 0.03);
}


void ship::draw(SDL_Surface *screen)
{
	if (image == NULL)
	{
		double xs = x-x_0, ys = y-y_0;

		if (xs < 0 || ys < 0 || xs > w || ys > h)
			return;
		double sl = 0.1;
		double dx, dy, bx, by, ex, ey;
		dx = sl*cos(alpha*3.1415926/180);
		dy = sl*sin(alpha*3.1415926/180);
		bx = xs-dx;
		by = ys-dy;	
		ex = xs+dx;
		ey = ys+dy;
		Draw_Line(screen, bx*WS/w, by*HS/h, ex*WS/w, ey*HS/h, 0xFFFFFF);	
		ex = xs+dx*3./4;
		ey = ys+dy*3./4;

		bx = -dy/4;
		by = dx/4;

		Draw_Line(screen, (ex+bx)*WS/w, (ey+by)*HS/h, (ex-bx)*WS/w, (ey-by)*HS/h, 0xFFFFFF);	
	}
	else
	{
		SDL_Rect rect;
		rect.w=image->w;
		rect.h=image->h;
		rect.x=(x-x_0)*WS/w-rect.w/2;
		rect.y=(y-y_0)*HS/h-rect.h/2;

		SDL_BlitSurface(image,0,screen,&rect);
	}


}


struct character:public ship
{
	character();
	void fire(void);
	void move(double dx, double dy);
	void chv();
};

void character::chv()
{
	if (x < x_0+0.1)
		x = x_0+0.1;

	if (x > x_0+w-0.1)
		x = x_0+w-0.1;

	if (y < 0.1)
		y = 0.1;
	if (y > h-0.1)
		y = h-0.1;
}


struct rocket:public ship
{
	void chv(){}
	rocket();
};


struct enemy:public ship
{
	int bt;
	int nbf, nbd;
	void fire();
	enemy();
	void chv();
	void activity();
	void on_die();
};


struct enemy_bomb:public ship
{
	enemy_bomb();
	
};


character *you;


character::character():ship()
{
	image=char_image;
}

void character::move(double dx, double dy)
{
	ship::move(dx, dy);
	chv();
}



void character::fire(void)
{
	rocket *newr = new rocket;
	newr->x = x+0.15;
	newr->y = y;
	newr->vx = 2*vx_0+vx;
	newr->vy = vy;
	newr->side=0;
	newr->alpha=0;
	ships.push_back(newr);
	ammo++;
}





rocket::rocket():ship()
{
	image = rocket_image;
}



enemy::enemy():ship()
{
	bt = rand()%1000;
	image = enemy_image;
	nbf = 3;
	nbd = 7;
}




void enemy::chv()
{
	vy = sin(time(NULL)+bt)*0.01;
	
}

void enemy::activity()
{
	if (rand()%(40-count%20) == 0)
		fire();
}

void enemy::on_die()
{
	for (int i = 0; i < nbd; i++)
	{
		enemy_bomb *newr = new enemy_bomb;
		newr->vx = 0.06*cos(i*2*3.14159/nbd);
		newr->vy = 0.06*sin(i*2*3.14159/nbd);
		
		newr->x = x+newr->vx*2;
		newr->y = y+newr->vy*2;
		
		
		newr->side=2;
		newr->alpha=i*180/nbd;
		ships.push_back(newr);	
	}
}


void enemy::fire(void)
{
	for (int i = 0; i < nbf; i++)
	{
		double a = alpha + (i-(nbf-1)/2.0)*40*2/(nbf-1);
		enemy_bomb *newr = new enemy_bomb;
		newr->x = x+0.15*cos(a*3.1415926535/180);
		newr->y = y+0.15*sin(a*3.1415926535/180);
		newr->vx = 0.01*cos(a*3.1415926535/180)-vx_0;
		newr->vy = 0.01*sin(a*3.1415926535/180);
		newr->side=2;
		newr->alpha=180;
		ships.push_back(newr);
	}
}

enemy_bomb::enemy_bomb():ship()
{
	image = enemy_bomb_image;
}





double mountain[2][10];
double pos0;



Uint32 timer(Uint32 interval, void *param)
{
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
			
			if (sides_inter[(*it)->side][(*it2)->side] == false)
				continue;
			if ((*it)->intersect(*it2))
			{
				(*it)->mr--;
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

Uint32 add_en(Uint32 interval, void *param)
{
	double y = (rand()%100)*h/100.;
	ship *en = new enemy;
	en->x = x_0+w-0.2;
	en->y = y;
	en->vx = -0.02;
	en->mr=1;
	en->vy = 0;
	en->alpha=180;
	en->side = 1;
	ships.push_back(en);
	return interval;
}



bool left, right, up, down;


SDL_Surface *load_image(const char *filename)
{	

	png_byte **row_pointers;
	char header[8];
	SDL_Surface *image = NULL;
	FILE *f = fopen(filename, "rb");
	fread(header, 1, 8, f);


	png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_info *info_ptr = png_create_info_struct(png_ptr);

	setjmp(png_jmpbuf(png_ptr));
	png_init_io(png_ptr, f);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	
	int width = png_get_image_width(png_ptr, info_ptr);
        int height = png_get_image_height(png_ptr, info_ptr);
        png_byte color_type = png_get_color_type(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	int number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);

	setjmp(png_jmpbuf(png_ptr));
	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);

	int y, x;
	for (y=0; y<height; y++)
		row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

	png_read_image(png_ptr, row_pointers);
	fclose(f);
	
	image = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	SDL_LockSurface(image);
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			((Uint32*)image->pixels)[y*image->w+x] = row_pointers[y][4*x+0]<<24 | 
					row_pointers[y][4*x+1]<<16 | 
					row_pointers[y][4*x+2]<<8 | 
					row_pointers[y][4*x+3];
			
		}
		free(row_pointers[y]);
	}
	free(row_pointers);
	SDL_UnlockSurface(image);
	
	
	return image;
}


void my_draw_line(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color)
{
	int dx=x2-x1, dy=y2-y1;
	double t;
	for (t = 0; t <= 1; t+=0.002)
	{
		int x = x1 + dx*t;
		int y = y1 + dy*t;
		if (x < 0 || y < 0 || x >= WS || y >= HS)
			continue;
		((Uint32*)(screen->pixels))[y*screen->w+x] = color;
	}

}

void draw_walls(SDL_Surface *screen)
{
	int i;
	int j;
	for (j = 0; j < 2; j++)
	for (i = 0; i <= 3; i++)
	{
		int x1, y1, x2, y2;
		double x_1, x_2;
		x_1 = i*w/3 + pos0;
		x_2 = (i+1)*w/3 + pos0;
		
		x1 = x_1*WS/w;
		x2 = x_2*WS/w;
		y1 = (mountain[j][i]-y_0)*HS/h;
		y2 = (mountain[j][i+1]-y_0)*HS/h;
		my_draw_line(screen, x1, y1, x2, y2, 0x0000FF00);
		
	}
}


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


void init_wall()
{
	for (int i = 0; i <= 4; i++)
	{
		mountain[0][i] = (rand()%(HS/4))*h/HS;
		mountain[1][i] = (rand()%(HS/4))*h/HS+h*3/4.;
	}
}

int main(void)
{
	SDL_Event event;


	TTF_Font *font;
	TTF_Init();
	font = TTF_OpenFont("./font.ttf",24);
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
	
	char_image=load_image("character.png");
	rocket_image=load_image("rocket.png");
	enemy_image=load_image("enemy.png");
	enemy_bomb_image=load_image("enemy_bomb.png");

	init_wall();


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
	
	SDL_TimerID enemy_timer_id = SDL_AddTimer(2000, add_en, NULL);
	SDL_TimerID my_timer_id = SDL_AddTimer(50, timer, NULL);


	while (!g_end)
	{	
		int ns = ships.size();
		int i;
		char StatusString[1000]={0};
		std::list<ship*>::iterator it, it2;
		SDL_FillRect(screen, NULL, 0x000000);
		for (it = ships.begin(); it != ships.end(); ++it)
		{
			(*it)->draw(screen);	
		}
			
		Draw_Line(screen, 0, HS, WS, HS, 0xFF00FF);
		

		sprintf(StatusString, "Count: %i   Ammo: %i    Objects: %i    Angry: %i", count, ammo, (int)ships.size(), count%20);
		imgTxt = TTF_RenderText_Solid(font, StatusString, fColor);
		SDL_BlitSurface(imgTxt, NULL, screen, &txtRect );
		
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
			      			case SDLK_LEFT:
							left = true;
							you->vx = vx_0-0.025;
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
			}
		}


		
		usleep(1e6/50);

		SDL_Flip(screen);
	}
	SDL_RemoveTimer(my_timer_id);
	SDL_RemoveTimer(enemy_timer_id);
	ships.clear();
	printf("Count: %i\n", count);
	SDL_Quit();
	return 0;
}


