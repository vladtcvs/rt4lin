#include <SDL.h>
#include <stdio.h>
#include <png.h>
#include "data_structures.h"
#include <math.h>

#define sqr(x) ((x)*(x))

extern double x_0, y_0;
extern double w, h;
extern int HS, WS;

void draw_object(SDL_Texture *image, double x, double y)
{
	SDL_Rect rect;
	int pw, ph;
	SDL_QueryTexture(image, NULL, NULL, &pw, &ph);
	rect.w=pw;
	rect.h=ph;
	rect.x=(x-x_0)*WS/w-rect.w/2;
	rect.y=(y-y_0)*HS/h-rect.h/2;

	SDL_RenderCopy(renderer,image, NULL, &rect);	
}


void draw_text(char *str, SDL_Color fcolor, TTF_Font *font, int x, int y)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;

	SDL_Surface *text = TTF_RenderText_Blended(font, str, fcolor);
	rect.w = text->w;
	rect.h = text->h;		
	SDL_Texture *txt = SDL_CreateTextureFromSurface(renderer, text);
	SDL_FreeSurface(text);
	SDL_RenderCopy(renderer, txt, NULL, &rect);
}


void clear_block(int x, int y, int bw, int bh)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = bw;
	rect.h = bh;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);
}

SDL_Texture *load_image(const char *filename)
{	

	png_byte **row_pointers;
	char header[8];
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
	


	Uint32 *pixels;

	int pitch = 0;
	
	SDL_Texture *image = NULL;
	image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	SDL_LockTexture(image, NULL, (void**)&pixels, &pitch);
	
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			Uint32 px;
			px = row_pointers[y][4*x+0]<<24 | 
					row_pointers[y][4*x+1]<<16 | 
					row_pointers[y][4*x+2]<<8 | 
					row_pointers[y][4*x+3];
			pixels[y*width+x] = px;
			
		}
		free(row_pointers[y]);
	}
	SDL_UnlockTexture(image);
	
	free(row_pointers);
	
	return image;
}




void load_images()
{
	char_image=load_image("character.png");
	rocket_image=load_image("rocket.png");
	enemy_image=load_image("enemy.png");
	enemy_bomb_image=load_image("enemy_bomb.png");
	life_bonus_image=load_image("life_bonus.png");

	int i;
	for (i = 0; i < ntrees; i++)
	{
		char fname[20];
		sprintf(fname, "tree_%i.png", i);
		trees[i] = load_image(fname);
	}
}


void draw_line(int x1, int y1, int x2, int y2, Uint32 color)
{
	 SDL_SetRenderDrawColor(renderer, (color>>24)&0xFF, (color>>16)&0xFF, (color>>8)&0xFF, color&0xFF);
	 SDL_RenderDrawLine(renderer,x1,y1,x2,y2);
}

void draw_walls()
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
		draw_line(x1, y1, x2, y2, 0x0000FF00);
		
	}
}


