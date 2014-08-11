#include "ship.h"
#include "data_structures.h"
#include <math.h>
#include <time.h>

#include "wall.h"

enemy::enemy():ship()
{
	side = O_ENEM;
	bt = rand()%1000;
	image = enemy_image;
	nbf = 2;
	nbd = 7;
	fire_vel = 0.06;
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
	if (eexpl == 0)
		return;
	for (int i = 0; i < nbd; i++)
	{
		enemy_bomb *newr = new enemy_bomb;
		newr->vx = fire_vel*cos(i*2*3.14159/nbd);
		newr->vy = fire_vel*sin(i*2*3.14159/nbd);
		
		newr->x = x+newr->vx*2;
		newr->y = y+newr->vy*2;	
		
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
		newr->vx = fire_vel*cos(a*3.1415926535/180)+vx;
		newr->vy = fire_vel*sin(a*3.1415926535/180)+vy;
		newr->alpha=180;
		ships.push_back(newr);
	}
}

