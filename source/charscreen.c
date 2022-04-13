#include "charscreen.h"

void drawChar(char c, int x, int y) {
	charScreen[x][y] = c;
}

void clearCharScreen() {
	for(int y = 0; y < CSCREEN_HEIGHT; y++) {
		for(int x = 0; x < CSCREEN_WIDTH; x++) {
			drawChar('A',x,y);	//Replace screen with some character
		}
	}
}

void renderCharScreen() {
	for(int y = 0; y < CSCREEN_HEIGHT; y++) {
		for(int x = 0; x < CSCREEN_WIDTH; x++) {
			printf("%c",charScreen[x][y]);
		}
		printf("\n");
	}
}
