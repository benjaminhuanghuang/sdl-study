#ifndef GAME_H
#define GAME_H

#include "Scene.h"

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
    void update();
    void render();
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
};

#endif // GAME_H