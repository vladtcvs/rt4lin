#include "data_structures.h"

double wall_here(double x, int l)
{
	if (l != 0 && l != 1)
		return -1;
	x = x-(x_0+pos0);

	int k = x/(w/3);
	int j;
		
	double y1 = mountain[l][k];
	double y2 = mountain[l][k+1];
	double dx = (x-k*w/3.)/(w/3);
	double ym = y1*(1-dx)+y2*dx;
	
	return ym;	
}


int wall_intersect(double x, double y, int l)
{
	if (l != 0 && l != 1)
		return -1;
	double ym = wall_here(x, l);
	
	if (l == 0 && y < ym+0.1)
		return 1;
	if (l == 1 && y > ym -0.1)
		return 1;
	return 0;
}

double drandom()
{
	return ((double)rand())/(RAND_MAX-1);
}
void wall_new()
{
	double kw = 0.45;
	if (pos0 < -w/3)
	{
		int i;
		for (i = 0; i <= 3; i++)
		{
			mountain[0][i]=mountain[0][i+1];
			mountain[1][i]=mountain[1][i+1];
		}
		mountain[0][4] = drandom()*kw*h;
		mountain[1][4] = h-drandom()*kw*h;
		pos0 = 0;
	}
}


