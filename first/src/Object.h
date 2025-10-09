#pragma once
#include<SDL2/SDL.h>

struct Player {
	SDL_Texture* texture = nullptr;
	SDL_FRect position = { 0,0 };
	int width = 0;
	int height = 0;
	int speed = 200;
	Uint32 coolDown = 500;
	Uint32 lastShootTime = 0;

};

struct ProjectilePlayer {

	SDL_Texture* texture = nullptr;
	SDL_FRect position = {0, 0};
	int width = 0;
	int height = 0;
	int speed = 400;
};

