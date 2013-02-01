#include "ship.h"
#include "data_structures.h"


int ammo = 0;


character::character():ship()
{
	side = O_CHAR;
	image=char_image;
	fire_vel = 0.1;
	mr = 3;
}

void character::move(double dx, double dy)
{
	ship::move(dx, dy);
	chv();
}


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


void character::fire(void)
{
	rocket *newr = new rocket;
	newr->x = x+0.15;
	newr->y = y;
	if (horiz == 0)
	{
		newr->vx = fire_vel+vx;
		newr->vy = vy;
	}
	else 
	{
		newr->vx = fire_vel+vx_0;
		newr->vy = 0;
	}
	newr->alpha=0;
	ships.push_back(newr);
	ammo++;
}


