#include "ship.h"
#include "data_structures.h"
#include "time.h"



enemy_bomb::enemy_bomb():ship()
{
	image = enemy_bomb_image;
	side = O_BOMB;
}



rocket::rocket():ship()
{
	image = rocket_image;
	side = O_YBOMB;
}



tree::tree():ship()
{
	image = trees[rand()%ntrees];
	side = O_WALL;
}

