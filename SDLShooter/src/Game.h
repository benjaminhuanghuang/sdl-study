#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "Object.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <map>

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
    SDL_Point renderTextCentered(std::string text, float posY, bool isTitle);
    void renderTextPos(std::string text, int posX, int posY, bool isLeft = true);

    void render();
    // setters
    void setFinalScore(int score) { finalScore = score; }
    // getters
    SDL_Window *getWindow() const { return window; }
    SDL_Renderer *getRenderer() const { return renderer; }
    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
    int getFinalScore() const { return finalScore; }
    std::multimap<int, std::string, std::greater<int>> &getLeaderBoard() { return leaderBoard; }
    void insertLeaderBoard(int score, std::string name);

private:
    Game();
    // Disable copy constructor and copy assignment operator
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    TTF_Font *titleFont;
    TTF_Font *textFont;

    bool isRunning = true;
    Scene *currentScene = nullptr;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;
    int FPS = 60;
    Uint32 frameTime;
    float deltaTime;
    int finalScore = 0;
    bool isFullscreen = false;

    Background nearStars;
    Background farStars;

    // score -> name, sorted by score in descending order
    std::multimap<int, std::string, std::greater<int>> leaderBoard;
    void backgroundUpdate(float deltaTime);
    void renderBackground();
    void saveData();
    void loadData();
};

#endif // GAME_H