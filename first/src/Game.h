#pragma once
#include <SDL2/SDL.h>
#include "Scene.h"
#include<SDL2/SDL_image.h>

class Game
{
public:
	static Game& getInstance() {
		static Game instanece;
		return instanece;
	}

	~Game();

	void run();
	void init();
    void clean();
	void changeScene(Scene* scene);
	
	//Getter
	SDL_Window* getWindow() { return window; }
	SDL_Renderer* getRenderer() { return renderer; }
	int getWindowWidth() { return windowWidth; }
	int getWindowHeight() { return windowHeight; }

	//Setter
	void setWindowWidth(int width) {
		windowWidth = width;
	}
	void setWindowHeight(int height) {
		windowHeight = height;
	}

	void handleEvent(SDL_Event* event);
	void update(float deltaTime);
	void render();

private:
	Game();
	//禁止拷贝构造和转换构造
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	Uint32 frameTime;
	int FPS = 60;
	float deltaTime;

	int windowWidth = 600;
	int windowHeight = 800;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	bool isRunning = true;
	Scene* currentScene=nullptr;

};

