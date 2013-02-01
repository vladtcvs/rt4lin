#include "ship.h"
#include "data_structures.h"
#include "time.h"
#include <math.h>

life_bonus::life_bonus():ship()
{
	image = life_bonus_image;
	side = O_LBONUS;
	bt = rand()%10;	
}


void life_bonus::chv()
{
	vy = sin(time(NULL)*1.3+bt)*0.03;
}


void life_bonus::activity()
{}
