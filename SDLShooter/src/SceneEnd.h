#ifndef SCENEEND_H
#define SCENEEND_H

#include "Scene.h"

class SceneEnd : public Scene
{
public:
    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    void handleEvent(SDL_Event *event) override;
};
#endif // SCENEEND_H