#pragma once
#include "scene.h"
#include <string>
#include <SDL2/SDL_mixer.h>

class SceneEnd : public Scene {
    virtual void init();
    virtual void update(float deltaTime);
    virtual void render();
    virtual void clean();
    virtual void handleEvent(SDL_Event* event);

private:
    bool isTyping = true;
    std::string name = "";
    float blinkTimer = 1.0f;  // 新增：光标闪烁计时器

    void renderPhase1();
    void renderPhase2();

    void removeLastUTF8Char(std::string& str);

    Mix_Music* bgm;
};