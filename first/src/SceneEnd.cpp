#include "SceneEnd.h"
#include "Game.h"
#include <string>

void SceneEnd::init()
{

    if (!SDL_IsTextInputActive()) {
        SDL_StartTextInput();
    }
    if (!SDL_IsTextInputActive()) {
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "SDL", "Failed to start text input: %s", SDL_GetError());
    }

}

void SceneEnd::update(float deltaTime)
{
    blinkTimer -= deltaTime;
    if (blinkTimer <= 0) {
        blinkTimer += 1.0f;
    }

}

void SceneEnd::render()
{

    if (isTyping) {
        renderPhase1();
    }
    else {
        renderPhase2();
    }

}

void SceneEnd::clean()
{
    if (SDL_IsTextInputActive()) {
        SDL_StopTextInput();
    }

}

void SceneEnd::handleEvent(SDL_Event* event)
{
    if (isTyping) {
        if (event->type == SDL_TEXTINPUT) {
            name += event->text.text;
        }
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
                isTyping = false;
                SDL_StopTextInput();
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                removeLastUTF8Char(name);  // 使用我们的新方法
            }
        }
    }
    else {
        // TODO: 处理后续逻辑
    }
}

void SceneEnd::renderPhase1()
{
    auto score = game.getFinalScore();
    std::string scoreText = "你的得分是：" + std::to_string(score);
    std::string gameOver = "Game Over";
    std::string instrutionText = "请输入你的名字，按回车键确认：";
    game.renderTextCentered(scoreText, 0.1, false);
    game.renderTextCentered(gameOver, 0.4, true);
    game.renderTextCentered(instrutionText, 0.6, false);

    if (name != "") {
        SDL_Point p = game.renderTextCentered(name, 0.8, false);
        if (blinkTimer < 0.5) {
            game.renderTextPos("_", p.x, p.y);
        }
    }
    else {
        if (blinkTimer < 0.5) {
            game.renderTextCentered("_", 0.8, false);
        }
    }
}

void SceneEnd::renderPhase2()
{
}

void SceneEnd::removeLastUTF8Char(std::string& str)
{
    if (str.empty()) return;

    auto lastchar = str.back();
    if ((lastchar & 0b10000000) == 0b10000000) { // 中文字符的后续字节
        str.pop_back();
        while ((str.back() & 0b11000000) != 0b11000000) { // 判断是否是中文字符的第一个字节
            str.pop_back();
        }
    }
    str.pop_back();
}