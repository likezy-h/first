#pragma once
#include<SDL2/SDL.h>

struct Player {
	SDL_Texture* texture = nullptr;
	SDL_FRect position = { 0,0 };
	int width = 0;
	int height = 0;
};

