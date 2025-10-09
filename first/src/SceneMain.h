#pragma once
#include "Scene.h"
#include"Game.h"
#include"Object.h"
#include<list>

class SceneMain :public Scene {

public:
	SceneMain();
	~SceneMain();

	void update(float deltaTime) override;
	void render() override;
	void handleEvent(SDL_Event* event) override;
	void init() override;
	void clean() override;

	void keyboradConntrol(float deltaTime);

	void shootPlayer();
	void updatePlayerProjectiles(float deltaTime);
	void renderPlayerProjectiles();

private:
	Game& game;
	Player player;

	//子弹的模板
	ProjectilePlayer projectilePlayerTemplate;

	//当前屏幕上的子弹
	std::list<ProjectilePlayer*> projectilesPlayer;

};