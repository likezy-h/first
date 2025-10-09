#include "SceneMain.h"

SceneMain::SceneMain():game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::update(float deltaTime)
{

	keyboradConntrol(deltaTime);
	updatePlayerProjectiles(deltaTime);
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

	renderPlayerProjectiles();
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

	//init projectilePlayer
	projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
	SDL_QueryTexture(projectilePlayerTemplate.texture, NULL, NULL, &projectilePlayerTemplate.width,&projectilePlayerTemplate.height);
	projectilePlayerTemplate.width /= 4;
	projectilePlayerTemplate.height / 4;

}

void SceneMain::clean()
{
	for (auto& projectile : projectilesPlayer) {
		if (projectile->texture != nullptr) {
			SDL_DestroyTexture(projectile->texture);
		}

	}
	projectilesPlayer.clear();

	if (projectilePlayerTemplate.texture) {
		SDL_DestroyTexture(projectilePlayerTemplate.texture);
	}

	if (player.texture) {
		SDL_DestroyTexture(player.texture);
	}
}



void SceneMain::keyboradConntrol(float deltaTime)
{
	auto keyboradState = SDL_GetKeyboardState(NULL);

	if (keyboradState[SDL_SCANCODE_W]) {
			player.position.y -= deltaTime * player.speed;
	}
	if (keyboradState[SDL_SCANCODE_A]) {
			player.position.x -= deltaTime * player.speed;
	}
	if (keyboradState[SDL_SCANCODE_S]) {
			player.position.y += deltaTime * player.speed;
	}
	if (keyboradState[SDL_SCANCODE_D]) {
			player.position.x += deltaTime * player.speed;
	}

	if (keyboradState[SDL_SCANCODE_J]) {
		auto currentTime = SDL_GetTicks();
		if (currentTime - player.lastShootTime > player.coolDown) {
				shootPlayer();
				player.lastShootTime = currentTime;
			}
		}

	if (player.position.x < 0) {
			player.position.x = 0;
	}
	if (player.position.x > game.getWindowWidth() - player.width) {
			player.position.x = game.getWindowWidth() - player.width;
	}
	if (player.position.y < 0) {
			player.position.y = 0;
	}
	if (player.position.y > game.getWindowHeight() - player.height) {
			player.position.y = game.getWindowHeight() - player.height;
	}

}
void SceneMain::shootPlayer()
{
	//创建新子弹
	auto projectile = new ProjectilePlayer(projectilePlayerTemplate);
	// 定位在飞机顶部中央
	projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
	projectile->position.y = player.position.y;
	// 添加到活动子弹列表
	projectilesPlayer.push_back(projectile);
}

void SceneMain::updatePlayerProjectiles(float deltaTime)
{
	int margin = 32;
	for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();) {
		auto projectile = (*it);
		projectile->position.y -= deltaTime * projectile->speed;

		if (projectile->position.y + margin < 0) {
			delete projectile;
			it = projectilesPlayer.erase(it);
		}
		else
		{
			++it;
		}

	}

}

void SceneMain::renderPlayerProjectiles()
{
	for (auto projectile : projectilesPlayer) {
		SDL_Rect projectileRect = {
			static_cast<int>(projectile->position.x),
			static_cast<int>(projectile->position.y),
			projectile->width,
			projectile->height
		};
		SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
	}
}
