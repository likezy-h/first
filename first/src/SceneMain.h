#pragma once

#include "Scene.h"
#include "Game.h"
#include "Object.h"
#include <list>
#include <random>

class SceneMain : public Scene {
public:

	SceneMain();
	~SceneMain();

	void update(float deltaTime) override;
	void render() override;
	void handleEvent(SDL_Event* event) override;
	void init() override;
	void clean() override;

	void keyboradControl(float deltaTime);

	void shootPlayer();
	void updatePlayerProjectiles(float deltaTime);
	void renderPlayerProjectiles();

	void spawEnemy();
	void updateEnemy(float deltaTime);
	void renderEnemy();

	void renderEnemyProjectiles();
	void updateEnemyProjectiles(float deltaTime);
	void shootEnemy(Enemy* enemy);
	SDL_FPoint getDirection(Enemy* enemy);
private:
	Game& game;
	Player player;

	std::mt19937 gen;
	std::uniform_real_distribution<float> dis;

	// 子弹的模板
	ProjectilePlayer projectilePlayerTemplate;

	// 当前屏幕上的子弹
	std::list<ProjectilePlayer*> projectilesPlayer;

	Enemy enemyTemplate;
	std::list<Enemy*> enemies;

	ProjectileEnemy projectileEnemyTemplate;
	std::list<ProjectileEnemy*> projectilesEnemy;
};