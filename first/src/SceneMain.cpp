#include "SceneMain.h"

SceneMain::SceneMain():game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::update()
{
}

void SceneMain::render()
{

	SDL_Rect rect = {
		static_cast<int>(player.position.x),
		static_cast<int>(player.position.y),
		player.width,
		player.height
	};

	SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &rect);

}

void SceneMain::handleEvent(SDL_Event* event)
{
}

void SceneMain::init()
{
	player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
	SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);

	player.height /= 4;
	player.width /= 4;
	player.position.x = game.getWindowWidth() / 2 - player.width / 2;
	player.position.y = game.getWindowHeight() - player.height;


}

void SceneMain::clean()
{

	if (player.texture) {
		SDL_DestroyTexture(player.texture);
	}
}
