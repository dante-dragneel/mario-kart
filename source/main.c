#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <wiringPi.h>
#include <pthread.h>
#include "framebuffer.h"
#include "input.h"
#include "draw.h"
#include "game_state.h"

#define REFRESH_TIME 1000000
#define NORMAL_SPEED 1
#define OFFROAD_SPEED 2

#define TIME_FOR_VALUEPACKS 30

#define R_HIGH 14
#define R_LOW 6

void game();
void startMenu();
void init_level_1();
void init_level_2();
void init_level_3();
void setLevel();
void setVisualArray();
void render();
void moveKart();
void moveEnemy(struct Object *);
void init_valuepacks();
void assignBG();
void restart();

struct GameState state;
int time_passed;
int option;
int menuOpen;
int play;
int kartMoving;
int activeBuff;

struct fbs framebufferstruct;

void *read_input(void *ptr)
{
	input();
}

int main()
{
	/* initialize + get FBS */
	framebufferstruct = initFbInfo();

	pthread_t input_thread;
	int res = pthread_create(&input_thread, NULL, read_input, "1");
	if (res)
	{
		fprintf(stderr, "Error creating input thread\n");
		return 1;
	}

	selectStart(RESX, RESY);
	menuOpen = 1;
	play = 1;
	option = 1;

	while (menuOpen == 1)
	{
		startMenu();
	}

	if (play == 1)
	{
		game();
	}

	pthread_join(input_thread, NULL);
	return 0;
}

void startMenu()
{
	if (button_pressed == 9)
	{
		if (option == 1)
		{
			menuOpen = 0;
			play = 1;
		}
		if (option == 2)
		{
			menuOpen = 0;
			play = 0;
			system("clear");
			exit(0);
		}
	}

	if (button_pressed == 5)
	{
		selectStart(RESX, RESY);
		option = 1;
	}
	else if (button_pressed == 6)
	{
		selectQuit(RESX, RESY);
		option = 2;
	}
}

void *processGameLogic()
{
	state.level = 1;
	state.lost = 0;
	state.won = 0;
	state.time_left = 120;
	state.lives = 5;

	int isRunning = 1;
	int showPauseMenu = 0;
	time_passed = 0;

	// Continues to process game logic till all the stages are cleared
	while (isRunning)
	{
		// Check if pause menu is on or not:
		if (showPauseMenu == 0)
		{
			if (button_pressed != -1)
			{
				kartMoving = 1;

				// Checking for start button to set showPauseMenu = 1
				if (button_pressed == 4)
				{
					showPauseMenu = 1;
					continue;
				}

				moveKart();

				if (state.speed == NORMAL_SPEED)
				{
					delayMicroseconds(200000);
				}
				else
				{
					delayMicroseconds(1000000);
				}

				render();
				kartMoving = 0;
			}

			if (state.lives <= 0 || state.time_left <= 0)
			{
				state.lost = 1;
			}
		}
		else if (showPauseMenu == 1)
		{
			exit(0);
			// TODO: Implement a pause menu
			showPauseMenu = 0;
			continue;
		}
	}
}

void *renderGameVisuals()
{
	while (1)
	{
		delayMicroseconds(REFRESH_TIME);
		for (int i = 0; i < MAX_ENEMIES; i++)
		{
			moveEnemy(&state.enemies[i]);

			// Check if collision with any of the enemies occurs
			if (equal(state.kart, state.enemies[i]))
			{
				restart();
				break;
			}
		}

		if (kartMoving != 1)
		{
			render();
		}
	}
}

void *runClock()
{
	while (state.time_left > 0)
	{
		delayMicroseconds(1000000);
		state.time_left -= 1;
		time_passed += 1;
		if (time_passed == TIME_FOR_VALUEPACKS)
		{
			init_valuepacks();
		}
	}
}

void game()
{

	// Initialize the GameState structure
	setLevel();
	clear(RESX, RESY);
	render();

	pthread_t renderingThread, gameLogic, gameClock;

	// Thread 1: Use mainly for rendering moving objects
	int err1 = pthread_create(&renderingThread, NULL, renderGameVisuals, NULL);

	// Thread 2: Used for processing input and changing game_state variables
	int err2 = pthread_create(&gameLogic, NULL, processGameLogic, NULL);

	// Thread 3: Used for the timer in the game
	int err3 = pthread_create(&gameClock, NULL, runClock, NULL);

	// Handling errors while creating threads
	if (err1)
	{
		printf("Failed to create rendering thread in Game method");
	}
	else if (err2)
	{
		printf("Failed to create game logic thread");
	}
	else if (err3)
	{
		printf("Error in creating a clock thread");
	}
}

void setLevel()
{
	if (state.level == 1)
	{
		init_level_3();
	}
	else if (state.level == 2)
	{
		init_level_2();
	}
	else if (state.level == 3)
	{
		// set the GameVisuals to represent the map3
	}
	else if (state.level == 4)
	{
		// set the GameVisuals to represent the map4
	}
}

void render()
{
	assignBG();
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		drawEnemy(state.enemies[i].x, state.enemies[i].y);
	}
	if (time_passed >= TIME_FOR_VALUEPACKS)
	{
		for (int i = 0; i < MAX_VALUEPACKS; i++)
		{
			if (state.valuepacks[i].type == POWER_STAR)
			{
				drawStar(state.valuepacks[i].x, state.valuepacks[i].y);
			}
			if (state.valuepacks[i].type == EXTRA_LIFE)
			{
				drawHeart(state.valuepacks[i].x, state.valuepacks[i].y);
			}
			if (state.valuepacks[i].type == EXTRA_TIME)
			{
				drawTime(state.valuepacks[i].x, state.valuepacks[i].y);
			}
		}
	}
	drawKart(state.kart.x, state.kart.y);
}

void assignBG()
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (state.map[i][j].type == ROAD)
			{
				drawRoad(i * CELL_SIZE, j * CELL_SIZE);
			}
			else if (state.map[i][j].type == GRASS)
			{
				drawGrass(i * CELL_SIZE, j * CELL_SIZE);
			}
			else if (state.map[i][j].type == OBSTACLE)
			{
				if (i % 2 == 0)
				{
					drawBanana(i, j);
				}
				else
				{
					drawPothole(i, j);
				}
			}
			else if (state.map[i][j].type == FINISH)
			{
				drawFinish(i, j);
			}
		}
	}
}

void moveKart()
{
	// Handling motion of the kart
	if (button_pressed == 8 && (state.kart.x + CELL_SIZE < RESX))
	{
		state.kart.x += CELL_SIZE;
	}
	if (button_pressed == 6 && (state.kart.y + CELL_SIZE < 20 * CELL_SIZE))
	{
		state.kart.y += CELL_SIZE;
	}
	if (button_pressed == 7 && (state.kart.x - CELL_SIZE >= 0))
	{
		state.kart.x -= CELL_SIZE;
	}
	if (button_pressed == 5 && (state.kart.y - CELL_SIZE >= 0))
	{
		state.kart.y -= CELL_SIZE;
	}

	int x_unit = state.kart.x / CELL_SIZE;
	int y_unit = state.kart.y / CELL_SIZE;
	if (state.map[x_unit][y_unit].type == GRASS)
	{
		state.speed = OFFROAD_SPEED;
	}
	else
	{
		state.speed = NORMAL_SPEED;
	}

	if (state.map[x_unit][y_unit].type == OBSTACLE)
	{
		restart();
	}

	if (state.map[x_unit][y_unit].type == FINISH)
	{
		state.level += 1;
		setLevel();
	}

	// Check if collision with any of the enemies occurs
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (equal(state.kart, state.enemies[i]) && activeBuff != POWER_STAR)
		{
			state.lives -= 1;
			setLevel();
			break;
		}
		else if (equal(state.kart, state.enemies[i]) && activeBuff == POWER_STAR)
		{
			activeBuff = -1;
			break;
		}
	}

	// Check if collision with any of the value pack occurs
	for (int i = 0; i < MAX_VALUEPACKS; i++)
	{
		if (val_equal(state.kart, state.valuepacks[i]))
		{
			activeBuff = state.valuepacks[i].type;
			state.valuepacks[i].type = -1;
			if (activeBuff == EXTRA_LIFE)
			{
				state.lives += 1;
				activeBuff = -1;
			}
			if (activeBuff == EXTRA_TIME)
			{
				state.time_left += 30;
				activeBuff = -1;
			}
		}
	}
}

void moveEnemy(struct Object *enemy)
{
	int r;
	while (1)
	{
		r = rand() % 4;

		if (r == 0)
		{
			if ((state.map[((*enemy).x + CELL_SIZE) / CELL_SIZE][(*enemy).y / CELL_SIZE].type != ROAD))
			{
				continue;
			}
			else
			{
				(*enemy).x += CELL_SIZE;
				break;
			}
		}
		else if (r == 1)
		{
			if ((state.map[((*enemy).x - CELL_SIZE) / CELL_SIZE][(*enemy).y / CELL_SIZE].type != ROAD))
			{
				continue;
			}
			else
			{
				(*enemy).x -= CELL_SIZE;
				break;
			}
		}
		else if (r == 2)
		{
			if ((state.map[(*enemy).x / CELL_SIZE][((*enemy).y + CELL_SIZE) / CELL_SIZE].type != ROAD))
			{
				continue;
				break;
			}
			else
			{
				(*enemy).y += CELL_SIZE;
			}
		}
		else if (r == 3)
		{
			if ((state.map[(*enemy).x / CELL_SIZE][((*enemy).y - CELL_SIZE) / CELL_SIZE].type != ROAD))
			{
				continue;
			}
			else
			{
				(*enemy).y -= CELL_SIZE;
				break;
			}
		}
	};
}

void init_valuepacks()
{
	for (int i = 0; i < MAX_VALUEPACKS; i++)
	{
		int x;
		int y;
		int type;

		x = rand() % 40;
		y = rand() % 20;
		type = rand() % 3;

		state.valuepacks[i].x = x;
		state.valuepacks[i].y = y;
		state.valuepacks[i].type = type;
	}
}

void init_level_1()
{
	state.score = (state.level * 10 + state.lives * 4 + state.time_left) * 100;
	state.speed = NORMAL_SPEED;

	// Setting up the road for level 1
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (j < R_LOW || j > R_HIGH)
			{
				state.map[i][j].type = GRASS;
			}
			else
			{
				state.map[i][j].type = ROAD;
			}
		}
	}

	// Randomly designing the obstacles
	for (int i = 0; i < MAX_OBSTACLES; i++)
	{
		int x = rand() % M;
		int y = R_LOW + (rand() % (R_HIGH - R_LOW));
		state.map[x][y].type = OBSTACLE;
	}

	// Setting up the finish line
	for (int i = 0; i < 9; i++)
	{
		state.map[M - 1][6 + i].type = FINISH;
	}

	// Unit (0,12) is the spawn for the kart
	state.kart.x = 0;
	state.kart.y = 9 * CELL_SIZE;

	// Manually designing the spawn points for the enemies
	state.enemies[0].x = 10 * CELL_SIZE;
	state.enemies[0].y = 11 * CELL_SIZE;

	state.enemies[1].x = 20 * CELL_SIZE;
	state.enemies[1].y = 11 * CELL_SIZE;

	state.enemies[2].x = 15 * CELL_SIZE;
	state.enemies[2].y = 12 * CELL_SIZE;

	state.enemies[3].x = 30 * CELL_SIZE;
	state.enemies[3].y = 13 * CELL_SIZE;

	state.enemies[4].x = 17 * CELL_SIZE;
	state.enemies[4].y = 13 * CELL_SIZE;
}

void init_level_2()
{
	state.score = (state.level * 10 + state.lives * 4 + state.time_left) * 100;
	state.speed = NORMAL_SPEED;

	// Setting up the road
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (j < R_LOW + 2 || j > R_HIGH - 2)
			{
				state.map[i][j].type = GRASS;
			}
			else
			{
				state.map[i][j].type = ROAD;
			}
		}
	}

	// Randomly designing the obstacles
	for (int i = 0; i < MAX_OBSTACLES; i++)
	{
		int x = rand() % M;
		int y = (R_LOW + 2) + (rand() % (R_HIGH - R_LOW - 4));
		state.map[x][y].type = OBSTACLE;
	}

	// Setting up the finish line
	for (int i = 0; i < 5; i++)
	{
		state.map[M - 1][8 + i].type = FINISH;
	}

	// Unit (0,12) is the spawn for the kart
	state.kart.x = 0;
	state.kart.y = 12 * CELL_SIZE;

	// Manually designing the spawn points for the enemies
	state.enemies[0].x = 10 * CELL_SIZE;
	state.enemies[0].y = 11 * CELL_SIZE;

	state.enemies[1].x = 20 * CELL_SIZE;
	state.enemies[1].y = 11 * CELL_SIZE;

	state.enemies[2].x = 15 * CELL_SIZE;
	state.enemies[2].y = 12 * CELL_SIZE;

	state.enemies[3].x = 30 * CELL_SIZE;
	state.enemies[3].y = 13 * CELL_SIZE;

	state.enemies[4].x = 17 * CELL_SIZE;
	state.enemies[4].y = 13 * CELL_SIZE;
}

void init_level_3()
{

	state.score = (state.level * 10 + state.lives * 4 + state.time_left) * 100;
	state.speed = NORMAL_SPEED;

	// Setting up the grass
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			state.map[i][j].type = GRASS;
		}
	}

	int j = 10;
	int dir = 1;
	// Setting up the road
	for (int i = 0; i < M; i++)
	{
		j += dir;
		state.map[i][j].type = ROAD;
		state.map[i][j + 1].type = ROAD;
		state.map[i][j + 2].type = ROAD;
		state.map[i][j + 3].type = ROAD;
		state.map[i][j + 4].type = ROAD;
		state.map[i][j + 5].type = ROAD;

		if (i % 20 == 0)
		{
			dir = -1;
		}
		else if (i % 10 == 0)
		{
			dir = 1;
		}
	}

	int placed_obstacles = 0;
	// Randomly designing the obstacles
	while (placed_obstacles != MAX_OBSTACLES)
	{
		int x = rand() % M;
		int y = rand() % N;
		if (state.map[x][y].type == ROAD)
		{
			state.map[x][y].type = OBSTACLE;
			placed_obstacles += 1;
		}
	}

	// Setting up the finish line
	for (int i = 0; i < 6; i++)
	{
		state.map[M - 1][10 + i].type = FINISH;
	}

	// Unit (0,12) is the spawn for the kart
	state.kart.x = 0;
	state.kart.y = 12 * CELL_SIZE;

	// Manually designing the spawn points for the enemies
	state.enemies[0].x = 5 * CELL_SIZE;
	state.enemies[0].y = 11 * CELL_SIZE;

	state.enemies[1].x = 20 * CELL_SIZE;
	state.enemies[1].y = 14 * CELL_SIZE;

	state.enemies[2].x = 13 * CELL_SIZE;
	state.enemies[2].y = 10 * CELL_SIZE;

	state.enemies[3].x = 30 * CELL_SIZE;
	state.enemies[3].y = 5 * CELL_SIZE;

	state.enemies[4].x = 17 * CELL_SIZE;
	state.enemies[4].y = 11 * CELL_SIZE;
}
void restart()
{
	state.lives -= 1;
	setLevel();
}
