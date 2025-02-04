#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"

class Game;

class SceneMain : public Scene
{
public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void update() override;
    void render() override;
    void clean() override;
    void handleEvent(SDL_Event *event) override;
    void keyboardControl();

private:
    Game &game;
    Player player;
};
#endif // SCENE_MAIN_H