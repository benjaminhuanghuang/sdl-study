#include "Game.h"
#include "SceneMain.h"

#include <SDL.h>
#include <SDL_image.h>

Game::Game() {}

Game::~Game()
{
    clean();
}

void Game::run()
{
    while (isRunning)
    {
        SDL_Event event;
        handleEvent(&event);
        update();
        render();
    }
}

void Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL initialized failed: %s", SDL_GetError());
        isRunning = false;
    }
    // Create window
    window = SDL_CreateWindow("SDL Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window created failed: %s", SDL_GetError());
        isRunning = false;
    }
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer created failed: %s", SDL_GetError());
        isRunning = false;
    }
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_Init failed: %s", IMG_GetError());
        isRunning = false;
    }
    isRunning = true;
    currentScene = new SceneMain();
    currentScene->init();
}

void Game::clean()
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    // Clean Image
    IMG_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene *scene)
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}

void Game::handleEvent(SDL_Event *event)
{
    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            isRunning = false;
        }
        currentScene->handleEvent(event);
    }
}
void Game::update()
{
    currentScene->update();
}
void Game::render()
{
    // Clear the screen
    SDL_RenderClear(renderer);
    // Draw the scene
    currentScene->render();
    // Show the screen
    SDL_RenderPresent(renderer);
}