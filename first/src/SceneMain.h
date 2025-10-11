#pragma once

#include "Scene.h"
#include "Game.h"
#include "Object.h"
#include <list>
#include <random>
#include <map>
#include <SDL2/SDL_ttf.h>
#include <string>

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

	void updatePlayer(float deltaTime);   // 更新玩家状态
	void enemyExplode(Enemy* enemy);      // 敌机爆炸效果

	void updateExplosions(float deltaTime);  // 更新爆炸动画
	void renderExplosions();                 // 渲染爆炸动画

	void dropItem(Enemy* enemy);
	void updateItems(float deltaTime);
	void playerGetItem(Item* item);
	void renderItems();

	void renderUI();
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

	bool isDead = false;                 // 玩家是否死亡

	Explosion explosionTemplate;            // 爆炸动画模板
	std::list<Explosion*> explosions;       // 存储活动爆炸动画的列表

	Item itemLifeTemplate;
	std::list<Item*> items;

	// 主音效
	Mix_Music* bgm;
	std::map<std::string, Mix_Chunk*> sounds; // 存储音效

	SDL_Texture* uiHealth;

	TTF_Font* scoreFont;
	int score = 0;
};