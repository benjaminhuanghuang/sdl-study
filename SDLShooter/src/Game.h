#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "Object.h"
class Game
{
public:
    static Game &getInstance()
    {
        static Game instance;
        return instance;
    }

    ~Game();
    void init();
    void run();
    void clean();
    void changeScene(Scene *scene);

    void handleEvent(SDL_Event *event);
    void update(float deltaTime);
    void backgroundUpdate(float deltaTime);
    void render();
    void renderBackground();
    SDL_Window *getWindow() const { return window; }
    SDL_Renderer *getRenderer() const { return renderer; }
    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }

private:
    Game();
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    bool isRunning = true;
    Scene *currentScene = nullptr;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;
    int FPS = 60;
    Uint32 frameTime;
    float deltaTime;

    Background nearStars;
    Background farStars;
};

#endif // GAME_H