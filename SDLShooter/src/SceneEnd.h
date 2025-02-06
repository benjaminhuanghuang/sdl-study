#ifndef SCENEEND_H
#define SCENEEND_H

#include "Scene.h"
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class SceneEnd : public Scene
{
public:
    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    void handleEvent(SDL_Event *event) override;

private:
    std::string playerName = "";
    bool isTyping = true;
    float blinkTimer = 1.0f;
    Mix_Music *bgm;

    void renderPhase1();
    void renderPhase2();
    void removeLastUTF8Char(std::string &str);
};
#endif // SCENEEND_H