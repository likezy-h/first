
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
		auto frameStart = SDL_GetTicks();


		SDL_Event event;

		handleEvent(&event);

		update(deltaTime);

		render();
		
		auto frameEnd = SDL_GetTicks();
		auto diff = frameEnd - frameStart;

		if (diff < frameTime) {
			SDL_Delay(frameTime - diff);
			deltaTime = frameTime / 1000.0f;
		}
		else {
			deltaTime = diff / 1000.0f;
		}

	}

}

void Game::init()
{
	frameTime = 1000 / FPS;

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

void Game::update(float deltaTime) {

	currentScene->update(deltaTime);

}
void Game::render() {

	SDL_RenderClear(renderer);

	currentScene->render();

	SDL_RenderPresent(renderer);

}