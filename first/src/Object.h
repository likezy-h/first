#pragma once
#include <SDL2/SDL.h>

struct Player
{
	SDL_Texture* texture = nullptr;
	SDL_FRect position = { 0,0 };
	int width = 0;
	int height = 0;
	int speed = 200;
	Uint32 coolDown = 500;
	Uint32 lastShootTime = 0;
	int currentHealth = 10;
	int maxHealth = 10;
};

struct ProjectilePlayer
{
	SDL_Texture* texture = nullptr;
	SDL_FRect position = { 0,0 };
	int width = 0;
	int height = 0;
	int speed = 400;
	int damage = 1;
};

struct Enemy
{
	SDL_Texture* texture = nullptr;
	SDL_FRect position = { 0,0 };
	int width = 0;
	int height = 0;
	int speed = 200;
	Uint32 coolDown = 2000;
	Uint32 lastShootTime = 0;
	int currentHealth = 1;
};

struct ProjectileEnemy
{
	SDL_Texture* texture = nullptr;
	SDL_FRect position = { 0,0 ,0,0 };
	SDL_FPoint direction = { 0, 0 };  // 子弹方向向量
	int width = 0;
	int height = 0;
	int speed = 400;
	int damage = 1;
};

struct Explosion {
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0, 0 };
	int width = 0;
	int height = 0;
	int currentFrame = 0;       // 当前帧索引
	int totlaFrame = 0;         // 总帧数
	Uint32 startTime = 0;       // 动画开始时间
	Uint32 FPS = 10;            // 帧率（每秒播放帧数）
};

// 物品类型枚举
enum class ItemType {
	Life,
	Shield,
	Time
};


// 物品结构体
struct Item {
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0, 0 };
	SDL_FPoint direction = { 0, 0 };
	int width = 0;
	int height = 0;
	int speed = 200;
	int bounceCount = 3;
	ItemType type = ItemType::Life;
};

struct Background {
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0, 0 };
	float offset = 0;
	int width = 0;
	int height = 0;
	int speed = 30;
};