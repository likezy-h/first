// SceneMain.cpp
#include "SceneMain.h"
#include <cmath>

SceneMain::SceneMain() : game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::update(float deltaTime)
{
	keyboradControl(deltaTime);
	spawEnemy();
	updateEnemy(deltaTime);
	updatePlayerProjectiles(deltaTime);
	updateEnemyProjectiles(deltaTime);
	updateItems(deltaTime);
	updateExplosions(deltaTime);
	updatePlayer(deltaTime);
}

void SceneMain::render()
{
	// 渲染玩家子弹
	renderPlayerProjectiles();
	//渲染敌机子弹
	renderEnemyProjectiles();
	// 渲染玩家
	if (!isDead) {
		SDL_Rect playerRect = { static_cast<int>(player.position.x),
						static_cast<int>(player.position.y),
						player.width,
						player.height };
		SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
	}
	// 渲染敌人
	renderEnemy();
	// 渲染物品
	renderItems();
	// 渲染爆炸效果
	renderExplosions();
	// 渲染UI
	renderUI();
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

	// 初始化爆炸动画模板
	explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/effect/explosion.png");
	SDL_QueryTexture(explosionTemplate.texture, NULL, NULL, &explosionTemplate.width, &explosionTemplate.heith);
	explosionTemplate.tolarFrame = explosionTemplate.width / explosionTemplate.heith;
	explosionTemplate.width = explosionTemplate.heith;

	// 初始化物品模板
	itemLifeTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bonus_life.png");
	SDL_QueryTexture(itemLifeTemplate.texture, NULL, NULL, &itemLifeTemplate.width, &itemLifeTemplate.height);
	itemLifeTemplate.width /= 4;
	itemLifeTemplate.height /= 4;

	// 读取音效资源
	sounds["player_shoot"] = Mix_LoadWAV("assets/sound/laser_shoot4.wav");
	sounds["enemy_shoot"] = Mix_LoadWAV("assets/sound/xs_laser.wav");
	sounds["player_explode"] = Mix_LoadWAV("assets/sound/explosion1.wav");
	sounds["enemy_explode"] = Mix_LoadWAV("assets/sound/explosion3.wav");
	sounds["hit"] = Mix_LoadWAV("assets/sound/eff11.wav");
	sounds["get_item"] = Mix_LoadWAV("assets/sound/eff5.wav");

	// 读取并播放背景音乐
	bgm = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
	if (bgm == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load music: %s", Mix_GetError());
	}
	Mix_PlayMusic(bgm, -1);

	// 读取ui Health
	uiHealth = IMG_LoadTexture(game.getRenderer(), "assets/image/Health UI Black.png");

	// 载入字体
	scoreFont = TTF_OpenFont("assets/font/VonwaonBitmap-12px.ttf", 24);
}

void SceneMain::clean()
{
	// 清理容器
	for (auto sound : sounds) {
		if (sound.second != nullptr) {
			Mix_FreeChunk(sound.second);
		}
	}
	sounds.clear();

	for (auto& projectile : projectilesPlayer) {
		if (projectile != nullptr) {
			delete projectile;
		}
	}
	projectilesPlayer.clear();

	for (auto& enemy : enemies) {
		if (enemy != nullptr) {
			delete enemy;
		}
	}
	enemies.clear();

	for (auto& projectile : projectilesEnemy) {
		if (projectile != nullptr) {
			delete projectile;
		}
	}
	projectilesEnemy.clear();

	for (auto& explosion : explosions) {
		if (explosion != nullptr) {
			delete explosion;
		}
	}
	explosions.clear();

	for (auto& item : items) {
		if (item != nullptr) {
			delete item;
		}
	}
	items.clear();

	// 清理ui
	if (uiHealth != nullptr) {
		SDL_DestroyTexture(uiHealth);
	}

	// 清理字体
	if (scoreFont != nullptr) {
		TTF_CloseFont(scoreFont);
	}

	// 清理模版
	if (player.texture != nullptr)
	{
		SDL_DestroyTexture(player.texture);
	}
	if (projectilePlayerTemplate.texture != nullptr)
	{
		SDL_DestroyTexture(projectilePlayerTemplate.texture);
	}
	if (enemyTemplate.texture != nullptr) {
		SDL_DestroyTexture(enemyTemplate.texture);
	}
	if (projectileEnemyTemplate.texture != nullptr) {
		SDL_DestroyTexture(projectileEnemyTemplate.texture);
	}
	if (explosionTemplate.texture != nullptr) {
		SDL_DestroyTexture(explosionTemplate.texture);
	}
	if (itemLifeTemplate.texture != nullptr) {
		SDL_DestroyTexture(itemLifeTemplate.texture);
	}

	// 清理音乐资源
	if (bgm != nullptr) {
		Mix_HaltMusic();
		Mix_FreeMusic(bgm);
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
	Mix_PlayChannel(0, sounds["player_shoot"], 0);
}

void SceneMain::updatePlayerProjectiles(float deltaTime)
{
	int margin = 32; // 子弹超出屏幕外边界的距离
	for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();) {
		auto projectile = *it;
		projectile->position.y -= projectile->speed * deltaTime;
		// 检查子弹是否超出屏幕
		if (projectile->position.y + margin < 0) {
			delete projectile;
			it = projectilesPlayer.erase(it);
		}
		else {
			bool hit = false;
			for (auto enemy : enemies) {
				SDL_Rect enemyRect = {
					static_cast<int>(enemy->position.x),
					static_cast<int>(enemy->position.y),
					enemy->width,
					enemy->height
				};
				SDL_Rect projectileRect = {
					static_cast<int>(projectile->position.x),
					static_cast<int>(projectile->position.y),
					projectile->width,
					projectile->height
				};
				if (SDL_HasIntersection(&enemyRect, &projectileRect)) {
					enemy->currentHealth -= projectile->damage;
					delete projectile;
					it = projectilesPlayer.erase(it);
					hit = true;
					Mix_PlayChannel(-1, sounds["hit"], 0);
					break;
				}
			}

			if (!hit) {
				++it;
			}

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
	Mix_PlayChannel(-1, sounds["enemy_shoot"], 0);
}

void SceneMain::updateEnemy(float deltaTime)
{
	auto currentTime = SDL_GetTicks();
	for (auto it = enemies.begin(); it != enemies.end();) {
		auto enemy = *it;
		enemy->position.y += enemy->speed * deltaTime;
		if (enemy->position.y > game.getWindowHeight()) {
			delete enemy;
			it = enemies.erase(it);
		}
		else {

			if (currentTime - enemy->lastShootTime > enemy->coolDown && isDead == false) {
				shootEnemy(enemy);
				enemy->lastShootTime = currentTime;
			}

			if (enemy->currentHealth <= 0) {
				enemyExplode(enemy);
				it = enemies.erase(it);
			}

			else {
				++it;
			}

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
	auto margin = 32;
	for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();) {
		auto projectile = *it;
		projectile->position.x += projectile->speed * projectile->direction.x * deltaTime;
		projectile->position.y += projectile->speed * projectile->direction.y * deltaTime;
		if (projectile->position.y > game.getWindowHeight() + margin ||
			projectile->position.y < -margin ||
			projectile->position.x < -margin ||
			projectile->position.x > game.getWindowWidth() + margin) {
			delete projectile;
			it = projectilesEnemy.erase(it);
		}
		else {
			SDL_Rect projectileRect = {
				static_cast<int>(projectile->position.x),
				static_cast<int>(projectile->position.y),
				projectile->width,
				projectile->height
			};
			SDL_Rect playerRect = {
				static_cast<int>(player.position.x),
				static_cast<int>(player.position.y),
				player.width,
				player.height
			};
			if (SDL_HasIntersection(&projectileRect, &playerRect) && !isDead) {
				player.currentHealth -= projectile->damage;
				delete projectile;
				it = projectilesEnemy.erase(it);
				Mix_PlayChannel(-1, sounds["hit"], 0);
			}
			else {
				++it;
			}
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

void SceneMain::updatePlayer(float deltaTime)
{
	if (isDead) {
		return;
	}

	if (player.currentHealth <= 0) {
		// 玩家死亡处理
		isDead = true;
		Mix_PlayChannel(-1, sounds["player_explode"], 0);
	}

	for (auto enemy : enemies) {
		SDL_Rect enemyRect = {
			static_cast<int>(enemy->position.x),
			static_cast<int>(enemy->position.y),
			enemy->width,
			enemy->height
		};
		SDL_Rect playerRect = {
			static_cast<int>(player.position.x),
			static_cast<int>(player.position.y),
			player.width,
			player.height
		};
		if (SDL_HasIntersection(&playerRect, &enemyRect)) {
			player.currentHealth -= 1;
			enemy->currentHealth = 0;
		}

	}

}

void SceneMain::enemyExplode(Enemy* enemy)
{
	auto currentTime = SDL_GetTicks();
	auto explosion = new Explosion(explosionTemplate);
	explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2;
	explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->heith / 2;
	explosion->startTime = currentTime;
	explosions.push_back(explosion);

	// 添加50%概率掉落物品
	if (dis(gen) < 0.5f) {
		dropItem(enemy);
	}
	Mix_PlayChannel(-1, sounds["enemy_explode"], 0);
	score += 10;  // 击败敌机获得10分
	delete enemy;
}

void SceneMain::updateExplosions(float)
{
	auto currentTime = SDL_GetTicks();
	for (auto it = explosions.begin(); it != explosions.end();)
	{
		auto explosion = *it;
		// 根据时间差和帧率计算当前帧索引
		explosion->currentFrame = (currentTime - explosion->startTime) * explosion->FPS / 1000;
		// 检查动画是否播放完毕
		if (explosion->currentFrame >= explosion->tolarFrame) {
			delete explosion;
			it = explosions.erase(it);
		}
		else {
			++it;
		}
	}
}

void SceneMain::renderExplosions()
{
	for (auto explosion : explosions)
	{
		// 定义源矩形（精灵图上的区域）
		SDL_Rect src = { explosion->currentFrame * explosion->width, 0, explosion->width, explosion->heith };
		// 定义目标矩形（屏幕上的位置）
		SDL_Rect dst = {
			static_cast<int>(explosion->position.x),
			static_cast<int>(explosion->position.y),
			explosion->width,
			explosion->heith,
		};
		// 绘制当前帧
		SDL_RenderCopy(game.getRenderer(), explosion->texture, &src, &dst);
	}
}

void SceneMain::dropItem(Enemy* enemy)
{
	auto item = new Item(itemLifeTemplate);
	item->position.x = enemy->position.x + enemy->width / 2 - item->width / 2;
	item->position.y = enemy->position.y + enemy->height / 2 - item->height / 2;

	// 生成随机方向
	float angle = dis(gen) * 2 * M_PI;
	item->direction.x = cos(angle);
	item->direction.y = sin(angle);

	items.push_back(item);
}

void SceneMain::updateItems(float deltaTime)
{
	for (auto it = items.begin(); it != items.end();)
	{
		auto item = *it;
		// 更新位置
		item->position.x += item->direction.x * item->speed * deltaTime;
		item->position.y += item->direction.y * item->speed * deltaTime;

		// 处理屏幕边缘反弹
		if (item->position.x < 0 && item->bounceCount > 0) {
			item->direction.x = -item->direction.x;
			item->bounceCount--;
		}
		if (item->position.x + item->width > game.getWindowWidth() && item->bounceCount > 0) {
			item->direction.x = -item->direction.x;
			item->bounceCount--;
		}
		if (item->position.y < 0 && item->bounceCount > 0) {
			item->direction.y = -item->direction.y;
			item->bounceCount--;
		}
		if (item->position.y + item->height > game.getWindowHeight() && item->bounceCount > 0) {
			item->direction.y = -item->direction.y;
			item->bounceCount--;
		}

		// 如果超出屏幕范围则删除
		if (item->position.x + item->width < 0 ||
			item->position.x > game.getWindowWidth() ||
			item->position.y + item->height < 0 ||
			item->position.y > game.getWindowHeight()) {
			delete item;
			it = items.erase(it);
		}
		else {
			// 检测物品与玩家的碰撞
			SDL_Rect itemRect = {
				static_cast<int>(item->position.x),
				static_cast<int>(item->position.y),
				item->width,
				item->height
			};
			SDL_Rect playerRect = {
				static_cast<int>(player.position.x),
				static_cast<int>(player.position.y),
				player.width,
				player.height
			};

			if (SDL_HasIntersection(&itemRect, &playerRect))
			{
				playerGetItem(item);
				delete item;
				it = items.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void SceneMain::playerGetItem(Item* item)
{

	if (item->type == ItemType::Life) {
		player.currentHealth += 1;
		if (player.currentHealth > player.maxHealth) {
			player.currentHealth = player.maxHealth;
		}
	}
	score += 5;  // 拾取物品获得5分
	Mix_PlayChannel(-1, sounds["get_item"], 0);
}

void SceneMain::renderItems()
{
	for (auto& item : items)
	{
		SDL_Rect itemRect = {
			static_cast<int>(item->position.x),
			static_cast<int>(item->position.y),
			item->width,
			item->height
		};
		SDL_RenderCopy(game.getRenderer(), item->texture, NULL, &itemRect);
	}
}

void SceneMain::renderUI()
{
	int x = 10;
	int y = 10;
	int size = 32;
	int offset = 40;
	SDL_SetTextureColorMod(uiHealth, 100, 100, 100); // 颜色减淡
	for (int i = 0; i < player.maxHealth; i++)
	{
		SDL_Rect rect = { x + i * offset, y, size, size };
		SDL_RenderCopy(game.getRenderer(), uiHealth, NULL, &rect);
	}
	SDL_SetTextureColorMod(uiHealth, 255, 255, 255); // reset color
	for (int i = 0; i < player.currentHealth; i++)
	{
		SDL_Rect rect = { x + i * offset, y, size, size };
		SDL_RenderCopy(game.getRenderer(), uiHealth, NULL, &rect);
	}

	// 渲染得分
	auto text = "SCORE:" + std::to_string(score);
	SDL_Color color = { 255, 255, 255, 255 };  // 白色
	SDL_Surface* surface = TTF_RenderUTF8_Solid(scoreFont, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(game.getRenderer(), surface);
	SDL_Rect rect = { game.getWindowWidth() - 10 - surface->w, 10, surface->w, surface->h };
	SDL_RenderCopy(game.getRenderer(), texture, NULL, &rect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}
