#include <SDL/SDL.h>
#include "GameLoop.h"

int main(int argc, char* argv[]) {
	GameLoop gameLoop;
	gameLoop.run();
	return 0;
}