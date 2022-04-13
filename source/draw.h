#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "framebuffer.h"
#include "grass.h"
#include "kart.h"
#include "road.h"
#include "enemy.h"
#include "start_select.h"
#include "quit_select.h"
#include "power_star.h"
#include "extra_life.h"
#include "time_boost.h"
#include "banana.h"
#include "pothole.h"

#define M 40
#define N 20
#define RESX 1280
#define RESY 720
#define CELL_SIZE 32

#define EXTRA_TIME 0
#define POWER_STAR 1
#define EXTRA_LIFE 2

#define GRASS 0
#define ROAD 1

/* Definitions */
typedef struct
{
	short int color;
	short int x, y;
} Pixel;

struct fbs framebufferstruct;

void drawPixel(Pixel *pixel);
void drawGrass(int, int);
void drawKart(int, int);
void drawStar(int, int);
void drawBanana(int, int);
void drawObstacle(int, int);
void drawPothole(int, int);

void drawFinish(int x_unit, int y_unit)
{

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	for (int y = y_unit * CELL_SIZE; y < y_unit * CELL_SIZE + CELL_SIZE; y++)
	{
		for (int x = x_unit * CELL_SIZE; x < x_unit * CELL_SIZE + CELL_SIZE; x++)
		{
			if (y_unit % 2 == 0)
			{
				pixel->color = 0x0000;
			}
			else
			{
				pixel->color = 0xFFFF;
			}
			pixel->x = x;
			pixel->y = y;

			drawPixel(pixel);
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void drawPothole(int x_unit, int y_unit)
{
	short int *ptr = (short int *)pothole.pixel_data;

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;

	for (int y = y_unit * CELL_SIZE; y < y_unit * CELL_SIZE + CELL_SIZE; y++)
	{
		for (int x = x_unit * CELL_SIZE; x < x_unit * CELL_SIZE + CELL_SIZE; x++)
		{
			pixel->color = ptr[i];
			pixel->x = x;
			pixel->y = y;

			drawPixel(pixel);
			i++;
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void drawBanana(int x_unit, int y_unit)
{
	short int *ptr = (short int *)banana.pixel_data;

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;

	for (int y = y_unit * CELL_SIZE; y < y_unit * CELL_SIZE + CELL_SIZE; y++)
	{
		for (int x = x_unit * CELL_SIZE; x < x_unit * CELL_SIZE + CELL_SIZE; x++)
		{
			if (ptr == 0x0000)
			{
			}
			else
			{
				pixel->color = ptr[i];
				pixel->x = x;
				pixel->y = y;

				drawPixel(pixel);
			}
			i++;
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void drawRoad(int x_pos, int y_pos)
{
	short int *roadPtr = (short int *)road.pixel_data;

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;
	for (int y = y_pos; y < y_pos + CELL_SIZE; y++)
	{
		for (int x = x_pos; x < x_pos + CELL_SIZE; x++)
		{

			pixel->color = roadPtr[i];
			pixel->x = x;
			pixel->y = y;

			drawPixel(pixel);
			i++;
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void drawGrass(int x_pos, int y_pos)
{
	short int *ptr = (short int *)grass.pixel_data;

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;
	for (int y = y_pos; y < y_pos + CELL_SIZE; y++)
	{
		for (int x = x_pos; x < x_pos + CELL_SIZE; x++)
		{

			pixel->color = ptr[i];
			pixel->x = x;
			pixel->y = y;

			drawPixel(pixel);
			i++;
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void drawStar(int x_unit, int y_unit)
{
	short int *starPtr = (short int *)power_star.pixel_data;

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;

	for (int y = y_unit * CELL_SIZE; y < y_unit * CELL_SIZE + CELL_SIZE; y++)
	{
		for (int x = x_unit * CELL_SIZE; x < x_unit * CELL_SIZE + CELL_SIZE; x++)
		{
			if (starPtr[i] == 0x0000)
			{
			}
			else
			{
				pixel->color = starPtr[i];
				pixel->x = x;
				pixel->y = y;

				drawPixel(pixel);
			}
			i++;
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void drawTime(int x_unit, int y_unit)
{
	short int *timePtr = (short int *)time_boost.pixel_data;

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;

	for (int y = y_unit * CELL_SIZE; y < y_unit * CELL_SIZE + CELL_SIZE; y++)
	{
		for (int x = x_unit * CELL_SIZE; x < x_unit * CELL_SIZE + CELL_SIZE; x++)
		{
			if (timePtr[i] == 0x0000)
			{
			}
			else
			{
				pixel->color = timePtr[i];
				pixel->x = x;
				pixel->y = y;

				drawPixel(pixel);
			}
			i++;
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void drawHeart(int x_unit, int y_unit)
{
	short int *heartPtr = (short int *)extra_life.pixel_data;

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;

	for (int y = y_unit * CELL_SIZE; y < y_unit * CELL_SIZE + CELL_SIZE; y++)
	{
		for (int x = x_unit * CELL_SIZE; x < x_unit * CELL_SIZE + CELL_SIZE; x++)
		{
			if (heartPtr[i] == 0x0000)
			{
			}
			else
			{
				pixel->color = heartPtr[i];
				pixel->x = x;
				pixel->y = y;

				drawPixel(pixel);
			}
			i++;
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void drawKart(int x_pos, int y_pos)
{
	short int *kartPtr = (short int *)kart.pixel_data;

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;
	for (int y = y_pos; y < y_pos + CELL_SIZE; y++)
	{
		for (int x = x_pos; x < x_pos + CELL_SIZE; x++)
		{

			pixel->color = kartPtr[i];
			pixel->x = x;
			pixel->y = y;

			drawPixel(pixel);
			i++;
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void drawEnemy(int x_pos, int y_pos)
{
	short int *kartPtr = (short int *)enemy.pixel_data;

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;

	for (int y = y_pos; y < y_pos + CELL_SIZE; y++)
	{
		for (int x = x_pos; x < x_pos + CELL_SIZE; x++)
		{
			pixel->color = kartPtr[i];
			pixel->x = x;
			pixel->y = y;

			drawPixel(pixel);
			i++;
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void clear(int max_x, int max_y)
{
	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;

	for (int y = 0; y < max_y; y++)
	{
		for (int x = 0; x < max_x; x++)
		{
			pixel->color = 0x0000;
			pixel->x = x;
			pixel->y = y;

			drawPixel(pixel);
			i++;
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void selectStart(int max_x, int max_y)
{
	short int *menuPtr = (short int *)start_select.pixel_data;

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;

	for (int y = 0; y < max_y; y++)
	{
		for (int x = 0; x < max_x; x++)
		{
			pixel->color = menuPtr[i];
			pixel->x = x;
			pixel->y = y;

			drawPixel(pixel);
			i++;
		}
	}

	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

void selectQuit(int max_x, int max_y)
{
	short int *menuPtr = (short int *)quit_select.pixel_data;

	/* initialize a pixel */
	Pixel *pixel;
	pixel = malloc(sizeof(Pixel));

	int i = 0;

	for (int y = 0; y < max_y; y++)
	{
		for (int x = 0; x < max_x; x++)
		{
			pixel->color = menuPtr[i];
			pixel->x = x;
			pixel->y = y;

			drawPixel(pixel);
			i++;
		}
	}
	/* free pixel's allocated memory */
	free(pixel);
	pixel = NULL;
	// munmap(framebufferstruct.fptr, framebufferstruct.screenSize);
}

/* Draw a pixel */
void drawPixel(Pixel *pixel)
{
	long int location = (pixel->x + framebufferstruct.xOff) * (framebufferstruct.bits / 8) +
						(pixel->y + framebufferstruct.yOff) * framebufferstruct.lineLength;
	*((unsigned short int *)(framebufferstruct.fptr + location)) = pixel->color;
}
