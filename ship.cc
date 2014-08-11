#include "ship.h"
#include "data_structures.h"
#include "draw.h"
#include <math.h>

ship::ship()
{
	image = NULL;
	mr = 1;
	vx = 0;
	vy = 0;
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


void ship::draw()
{
	if (image == NULL)
	{
	}
	else
	{
		draw_object(image, x, y);
	}


}


