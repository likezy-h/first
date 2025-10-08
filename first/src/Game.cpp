
#include "Game.h"
#include"SceneMain.h"

Game::Game() {

}

Game::~Game() {

	clean();

}



void Game::run()
{

	while (isRunning)
	{
		SDL_Event event;

		handleEvent(&event);

		update();

		render();
		
	}

}

void Game::init()
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

		isRunning = false;

	}

	window = SDL_CreateWindow("Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	if (!window) {

		isRunning = false;

	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {

		isRunning = false;

	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {

		isRunning = false;
		
	}

	currentScene = new SceneMain();
	currentScene->init();

}

void Game::clean()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::changeScene(Scene* scene)
{

	if (currentScene != nullptr) {
		currentScene->clean();
		delete currentScene;
	}
	currentScene = scene;
	currentScene->init();

}



void Game::handleEvent(SDL_Event* event) {

	if (SDL_PollEvent(event)) {
		if (event->type == SDL_QUIT) {
			isRunning = false;
		}
		currentScene->handleEvent(event);

	}

}

void Game::update() {

	currentScene->update();

}
void Game::render() {

	SDL_RenderClear(renderer);

	currentScene->render();

	SDL_RenderPresent(renderer);

}