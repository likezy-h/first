#include "Game.h"
#include "SceneMain.h"

Game::Game()
{
}

Game::~Game()
{

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
			deltaTime = frameTime / 1000.f;
		}
		else
		{
			deltaTime = diff / 1000.0f;
		}

	}
}

void Game::init()
{
	frameTime = 1000 / FPS;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL Init Error:" << SDL_GetError();
		isRunning = false;
	}

	window = SDL_CreateWindow("Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "SDL Create window Error:" << SDL_GetError();
		isRunning = false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cerr << "SDL Create rendere Error:" << SDL_GetError();
		isRunning = false;
	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::cerr << "SDL_image Init Error:" << SDL_GetError();
		isRunning = false;
	}

	// 初始化SDL_mixer
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		isRunning = false;
	}

	// 打开音频设备
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not open audio! SDL_mixer Error: %s\n", Mix_GetError());
		isRunning = false;
	}

	// 设置音效channel数量
	Mix_AllocateChannels(32);

	// 设置音乐音量
	Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
	Mix_Volume(-1, MIX_MAX_VOLUME / 8);

	// 初始化背景卷轴
	nearStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-A.png");
	SDL_QueryTexture(nearStars.texture, NULL, NULL, &nearStars.width, &nearStars.height);
	nearStars.height /= 2;
	nearStars.width /= 2;

	farStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-B.png");
	SDL_QueryTexture(farStars.texture, NULL, NULL, &farStars.width, &farStars.height);
	farStars.height /= 2;
	farStars.width /= 2;
	farStars.speed = 20;  // 远处的星星移动速度较慢

	if (TTF_Init() == -1) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_ttf could not init! SDL_ttf Error: %s\n", SDL_GetError());
		isRunning = false;
	}

	currentScene = new SceneMain();
	currentScene->init();

}

void Game::clean()
{

	// 清理SDL_mixer
	Mix_CloseAudio();
	Mix_Quit();

	if (nearStars.texture != nullptr) {
		SDL_DestroyTexture(nearStars.texture);
	}
	if (farStars.texture != nullptr) {
		SDL_DestroyTexture(farStars.texture);
	}

	// 清理SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}

void Game::changeScene(Scene* scene)
{

	if (currentScene != nullptr)
	{
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

	backgroundUpdate(deltaTime);
	currentScene->update(deltaTime);

}

void Game::render() {

	// 清空
	SDL_RenderClear(renderer);
	// 渲染星空背景
	renderBackground();

	currentScene->render();
	// 显示更新
	SDL_RenderPresent(renderer);

}

void Game::backgroundUpdate(float deltaTime)
{

	nearStars.offset += nearStars.speed * deltaTime;
	if (nearStars.offset >= 0)
	{
		nearStars.offset -= nearStars.height;
	}

	farStars.offset += farStars.speed * deltaTime;
	if (farStars.offset >= 0) {
		farStars.offset -= farStars.height;
	}

}

void Game::renderBackground()
{

	// 渲染远处的星星
	for (int posY = static_cast<int>(farStars.offset); posY < getWindowHeight(); posY += farStars.height) {
		for (int posX = 0; posX < getWindowWidth(); posX += farStars.width) {
			SDL_Rect ds = { posX, posY, farStars.width, farStars.height };
			SDL_RenderCopy(renderer, farStars.texture, NULL, &ds);
		}
	}
	// 渲染近处的星星
	for (int posY = static_cast<int>(nearStars.offset); posY < getWindowHeight(); posY += nearStars.height)
	{
		for (int posX = 0; posX < getWindowWidth(); posX += nearStars.width)
		{
			SDL_Rect dstRect = { posX, posY, nearStars.width, nearStars.height };
			SDL_RenderCopy(renderer, nearStars.texture, nullptr, &dstRect);
		}
	}

}