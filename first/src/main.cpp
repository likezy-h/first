#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Game.h"

#undef main
int main(int, char**) {
	Game& game = Game::getInstance();
	game.init();
	game.run();

	return 0;
}