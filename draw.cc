#include <SDL.h>
#include <stdio.h>
#include <png.h>
#include "data_structures.h"
#include <math.h>

#define sqr(x) ((x)*(x))

extern double x_0, y_0;
extern double w, h;
extern int HS, WS;

void draw_object(SDL_Surface *screen, SDL_Surface *image, double x, double y)
{
	SDL_Rect rect;
	rect.w=image->w;
	rect.h=image->h;
	rect.x=(x-x_0)*WS/w-rect.w/2;
	rect.y=(y-y_0)*HS/h-rect.h/2;

	SDL_BlitSurface(image,0,screen,&rect);	
}



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





void load_images()
{
	char_image=load_image("character.png");
	rocket_image=load_image("rocket.png");
	enemy_image=load_image("enemy.png");
	enemy_bomb_image=load_image("enemy_bomb.png");

	int i;
	for (i = 0; i < ntrees; i++)
	{
		char fname[20];
		sprintf(fname, "tree_%i.png", i);
		trees[i] = load_image(fname);
	}
}


void my_draw_line(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color)
{
	int dx=x2-x1, dy=y2-y1;
	double t;
	double l = sqrt(sqr(x1-x2)+sqr(y1-y2));
	for (t = 0; t <= 1; t+=1/l)
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


