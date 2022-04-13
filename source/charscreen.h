#pragma once

#include <stdio.h>

#define clearConsole() printf("\e[1;1H\e[2J")
#define CSCREEN_WIDTH 50
#define CSCREEN_HEIGHT 50
char charScreen[CSCREEN_WIDTH][CSCREEN_HEIGHT];

void drawChar(char c, int x, int y);
void clearCharScreen();
void renderCharScreen();
