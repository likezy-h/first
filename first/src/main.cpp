#include "Game.h"
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>

#undef main


int main(int,char**) {

	Game& game=Game::getInstance();
	game.init();
	game.run();

	return 0;
}