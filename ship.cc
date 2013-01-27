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
		my_draw_line(screen, bx*WS/w, by*HS/h, ex*WS/w, ey*HS/h, 0xFFFFFF);	
		ex = xs+dx*3./4;
		ey = ys+dy*3./4;

		bx = -dy/4;
		by = dx/4;

		my_draw_line(screen, (ex+bx)*WS/w, (ey+by)*HS/h, (ex-bx)*WS/w, (ey-by)*HS/h, 0xFFFFFF);	
	}
	else
	{
		draw_object(screen, image, x, y);
	}


}


