#include "ship.h"
#include "data_structures.h"
#include "time.h"
#include <math.h>

#include "wall.h"

life_bonus::life_bonus():ship()
{
	image = life_bonus_image;
	side = O_LBONUS;
	bt = rand()%10;	
}


void life_bonus::chv()
{
	double y1 = wall_here(x,0);
	double y2 = wall_here(x,1);
	if (y < y1+0.1)
		y = y1+0.1;
	if (y > y2-0.1)
		y = y2-0.1;

	vy = sin(time(NULL)*1.3+bt)*0.03;
}


void life_bonus::activity()
{}
