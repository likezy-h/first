#pragma once
#include <iostream>

#include "Scene.h"
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "Object.h"
#include <SDL2/SDL_ttf.h>
#include <map>
#include <fstream>


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

	// Getter
	SDL_Window* getWindow() { return window; }
	SDL_Renderer* getRenderer() { return renderer; }
	int getWindowWidth() { return windowWidth; }
	int getWindowHeight() { return windowHeight; }
	int getFinalScore() { return finalScore; }

	// Setter
	void setWindowWidth(int width) { windowWidth = width; }
	void setWindowHeight(int height) { windowHeight = height; }
	void setFinalScore(int score) { finalScore = score; }

	void handleEvent(SDL_Event* event);
	void update(float deltaTime);
	void render();

	void backgroundUpdate(float deltaTime);
	void renderBackground();

	// 渲染文字函数，返回文本结束位置的坐标点
	SDL_Point renderTextCentered(std::string text, float posY, bool isTitle);
	void renderTextPos(std::string text, int posX, int posY, bool isLeft);
	void renderTextPos(std::string text, int posX, int posY);

	void insertLeaderBoard(int score, std::string name); // 插入排行榜
	std::multimap<int, std::string, std::greater<int>>& getLeaderBoard() { return leaderBoard; } // 返回leaderBoard的引用

	void saveData();
	void loadData();
private:
	Game();
	// 禁止拷贝构造和转换构造。
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	Uint32 frameTime; // 我们希望的帧时间
	int FPS = 60; // 
	float deltaTime;

	int windowWidth = 600;
	int windowHeight = 800;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	bool isRunning = true;
	Scene* currentScene = nullptr;

	Background nearStars;  // 近处的星星
	Background farStars;   // 远处的星星

	TTF_Font* titleFont;
	TTF_Font* textFont;

	int finalScore = 0;

	std::multimap<int, std::string, std::greater<int>> leaderBoard;// 使用multimap来存储分数和名字，分数作为键

	bool isFullscreen = false;
};

