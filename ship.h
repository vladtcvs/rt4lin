#ifndef SHIP_H
#define SHIP_H

#include <SDL.h>
#include "draw.h"


struct ship
{
	Texture *image;
	int mr;
	int side;
	double x, y;
	double alpha;
	double vx, vy;
	virtual void draw();
	virtual void move(double dx, double dy);
	void step(void);
	virtual void chv(void){}
	virtual void activity(){}
	virtual void on_die(){}
	virtual void fire(void){}
	virtual bool intersect(ship *other);
	ship();
};


struct character:public ship
{
	double vy1;
	double fire_vel;
	character();
	void fire(void);
	void move(double dx, double dy);
	void chv();
};

struct rocket:public ship
{
	void chv(){}
	rocket();
};


struct tree:public ship
{
	tree();
};

struct enemy:public ship
{
	int bt;
	int nbf, nbd;
	double fire_vel;
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

struct life_bonus:public ship
{
	int bt;
	life_bonus();
	void chv();
	void activity();
};




#endif
