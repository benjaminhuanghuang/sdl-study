#ifndef SCENEEND_H
#define SCENEEND_H

#include "Scene.h"
#include <string>
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
    void renderPhase1();
    void renderPhase2();
    void removeLastUTF8Char(std::string &str);
};
#endif // SCENEEND_H