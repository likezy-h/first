// SceneMain.cpp
#include "SceneMain.h"

SceneMain::SceneMain() : game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::update(float deltaTime)
{

	spawEnemy();
	keyboradControl(deltaTime);
	updatePlayerProjectiles(deltaTime);

	updateEnemyProjectiles(deltaTime);
	updateEnemy(deltaTime);
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

	renderEnemy();
	renderPlayerProjectiles();
	renderEnemyProjectiles();
}

void SceneMain::handleEvent(SDL_Event* event)
{
}

void SceneMain::init()
{

	std::random_device rd;
	gen = std::mt19937(rd());
	dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

	player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
	if (!player.texture) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player texture: %s", SDL_GetError());
	}

	SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);

	player.height /= 4;
	player.width /= 4;
	player.position.x = game.getWindowWidth() / 2 - player.width / 2;
	player.position.y = game.getWindowHeight() - player.height;

	// init projectilePlayer
	projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
	SDL_QueryTexture(projectilePlayerTemplate.texture, NULL, NULL, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
	projectilePlayerTemplate.width /= 4;
	projectilePlayerTemplate.height /= 4;

	enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-2.png");
	SDL_QueryTexture(enemyTemplate.texture, NULL, NULL, &enemyTemplate.width, &enemyTemplate.height);
	enemyTemplate.width /= 4;
	enemyTemplate.height /= 4;

	// 初始化敌机子弹模板
	projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-1.png");
	SDL_QueryTexture(projectileEnemyTemplate.texture, NULL, NULL, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
	projectileEnemyTemplate.width /= 4;
	projectileEnemyTemplate.height /= 4;

}

void SceneMain::clean()
{
	for (auto& projectile : projectilesPlayer) {
		if (projectile->texture != nullptr) {
			SDL_DestroyTexture(projectile->texture);
		}
	}
	projectilesPlayer.clear();

	for (auto& enemy : enemies) {
		if (enemy->texture != nullptr) {
			SDL_DestroyTexture(enemy->texture);
		}
	}
	enemies.clear();

	// 清理敌机
	for (auto& enemy : enemies) {
		if (enemy != nullptr) {
			delete enemy;
		}
	}
	enemies.clear();

	if (projectilePlayerTemplate.texture) {
		SDL_DestroyTexture(projectilePlayerTemplate.texture);
	}
	if (player.texture) {
		SDL_DestroyTexture(player.texture);
	}
	if (enemyTemplate.texture)
	{
		SDL_DestroyTexture(enemyTemplate.texture);
	}
	if (projectileEnemyTemplate.texture != nullptr) {
		SDL_DestroyTexture(projectileEnemyTemplate.texture);
	}

}

void SceneMain::keyboradControl(float deltaTime)
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
	// 创建新子弹
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

void SceneMain::spawEnemy()
{
	if (dis(gen) > 1 / 60.0f) {
		return;
	}

	auto enemy = new Enemy(enemyTemplate);

	enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
	enemy->position.y = -enemy->height;

	enemies.push_back(enemy);

}

void SceneMain::updateEnemy(float deltaTime)
{
	auto currentTime = SDL_GetTicks();
	int margin = 32;
	for (auto it = enemies.begin(); it != enemies.end();) {
		auto enemy = (*it);
		enemy->position.y += deltaTime * enemy->speed;

		if (enemy->position.y > game.getWindowHeight() + margin) {
			delete enemy;
			it = enemies.erase(it);
		}
		else
		{
			if (currentTime - enemy->lastShootTime > enemy->coolDown) {
				shootEnemy(enemy);
				enemy->lastShootTime = currentTime;
			}
			++it;
		}

	}
}

void SceneMain::renderEnemy()
{
	for (auto enemy : enemies) {
		SDL_Rect enemyRect = {
			static_cast<int>(enemy->position.x),
			static_cast<int>(enemy->position.y),
			enemy->width,
			enemy->height
		};
		SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);
	}

}

void SceneMain::renderEnemyProjectiles()
{
	for (auto projectile : projectilesEnemy) {
		SDL_Rect projectileRect = {
			static_cast<int>(projectile->position.x),
			static_cast<int>(projectile->position.y),
			projectile->width,
			projectile->height
		};

		// 计算子弹旋转角度（从弧度转换为角度，并调整为正确的方向）
		float angle = atan2(projectile->direction.y, projectile->direction.x) * 180 / M_PI - 90;

		// 使用带旋转参数的渲染函数
		SDL_RenderCopyEx(game.getRenderer(), projectile->texture, NULL, &projectileRect, angle, NULL, SDL_FLIP_NONE);
	}
}

void SceneMain::updateEnemyProjectiles(float deltaTime)
{

	auto margin = 32; // 屏幕边缘额外余量
	for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();) {
		auto projectile = *it;

		// 根据方向向量更新子弹位置
		projectile->position.x += projectile->speed * projectile->direction.x * deltaTime;
		projectile->position.y += projectile->speed * projectile->direction.y * deltaTime;

		// 检查子弹是否飞出屏幕边界
		if (projectile->position.y > game.getWindowHeight() + margin ||
			projectile->position.y < -margin ||
			projectile->position.x < -margin ||
			projectile->position.x > game.getWindowWidth() + margin) {
			delete projectile;
			it = projectilesEnemy.erase(it);
		}
		else {
			++it;
		}
	}

}

void SceneMain::shootEnemy(Enemy* enemy)
{
	// 创建新子弹
	auto projectile = new ProjectileEnemy(projectileEnemyTemplate);

	// 设置子弹初始位置（从敌机中央发射）
	projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
	projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;

	// 计算子弹飞向玩家的方向
	projectile->direction = getDirection(enemy);

	// 添加到敌机子弹列表
	projectilesEnemy.push_back(projectile);

}

SDL_FPoint SceneMain::getDirection(Enemy* enemy)
{
	// 计算从敌机中心到玩家中心的向量
	auto x = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
	auto y = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);

	// 计算向量长度
	auto length = sqrt(x * x + y * y);

	// 归一化向量
	x /= length;
	y /= length;

	return SDL_FPoint{ x, y };
}
