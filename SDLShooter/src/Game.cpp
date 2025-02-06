#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

Game::Game() {}

Game::~Game()
{
    clean();
}

void Game::run()
{
    while (isRunning)
    {
        auto frameStart = SDL_GetTicks();
        SDL_Event event;
        handleEvent(&event);
        update(deltaTime);
        render();
        auto frameEnd = SDL_GetTicks();
        auto diff = frameEnd - frameStart;
        if (diff < frameTime)
        {
            SDL_Delay(frameTime - diff);
            deltaTime = frameTime / 1000.0f;
        }
        else
        {
            deltaTime = diff / 1000.0f;
        }
    }
}

void Game::init()
{
    frameTime = 1000 / FPS;

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
    // Init music
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_Init failed: %s", Mix_GetError());
        isRunning = false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_OpenAudio failed: %s", Mix_GetError());
        isRunning = false;
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4); // volume of bgm
    Mix_AllocateChannels(16);
    Mix_Volume(-1, MIX_MAX_VOLUME / 4); // volume of sound effect

    if (TTF_Init() == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_Init failed: %s", TTF_GetError());
        isRunning = false;
    }
    // Init background
    nearStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-A.png");
    SDL_QueryTexture(nearStars.texture, nullptr, nullptr, &nearStars.width, &nearStars.height);
    farStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-B.png");
    SDL_QueryTexture(farStars.texture, nullptr, nullptr, &farStars.width, &farStars.height);
    farStars.speed = 20;

    titleFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 64);
    textFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 32);

    isRunning = true;
    currentScene = new SceneTitle();
    currentScene->init();
}

void Game::clean()
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    if (nearStars.texture != nullptr)
    {
        SDL_DestroyTexture(nearStars.texture);
    }
    if (farStars.texture != nullptr)
    {
        SDL_DestroyTexture(farStars.texture);
    }
    // Clean Image
    IMG_Quit();
    // Clean Mixer
    Mix_CloseAudio();
    Mix_Quit();

    // Clean TTF
    TTF_Quit();

    // Clean SDL
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
void Game::update(float deltaTime)
{
    backgroundUpdate(deltaTime);
    currentScene->update(deltaTime);
}
void Game::backgroundUpdate(float deltaTime)
{
    nearStars.offset += deltaTime * nearStars.speed;
    if (nearStars.offset >= 0)
    {
        nearStars.offset -= nearStars.height;
    }

    farStars.offset += deltaTime * farStars.speed;
    if (farStars.offset >= 0)
    {
        farStars.offset -= farStars.height;
    }
}
void Game::render()
{
    // Clear the screen
    SDL_RenderClear(renderer);

    renderBackground();
    // Draw the scene
    currentScene->render();
    // Show the screen
    SDL_RenderPresent(renderer);
}

void Game::renderBackground()
{
    for (int posY = static_cast<int>(farStars.offset); posY < getWindowHeight(); posY += farStars.height)
    {
        for (int posX = 0; posX < getWindowWidth(); posX += farStars.width)
        {
            SDL_Rect dstRect = {posX, posY, farStars.width, farStars.height};
            SDL_RenderCopy(renderer, farStars.texture, nullptr, &dstRect);
        };
    }

    for (int posY = static_cast<int>(nearStars.offset); posY < getWindowHeight(); posY += nearStars.height)
    {
        for (int posX = 0; posX < getWindowWidth(); posX += nearStars.width)
        {
            SDL_Rect dstRect = {posX, posY, nearStars.width, nearStars.height};
            SDL_RenderCopy(renderer, nearStars.texture, nullptr, &dstRect);
        };
    }
}

SDL_Point Game::renderTextCentered(std::string text, float posY, bool isTitle)
{
    SDL_Color color = {255, 255, 255, 255};
    TTF_Font *font = isTitle ? titleFont : textFont;
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int y = static_cast<int>(getWindowHeight() - surface->h) * posY;
    SDL_Rect dstRect = {getWindowWidth() / 2 - surface->w / 2, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    return {dstRect.x + dstRect.w, y};
}

void Game::renderTextPos(std::string text, int posX, int posY, bool isLeft = true)
{
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(textFont, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect;
    if (isLeft)
    {
        dstRect = {posX, posY, surface->w, surface->h};
    }
    else
    {
        dstRect = {getWindowWidth() - posX - surface->w, posY, surface->w, surface->h};
    }

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Game::insertLeaderBoard(int score, std::string name)
{
    leaderBoard.insert({score, name});

    if (leaderBoard.size() > 8)
    {
        leaderBoard.erase(std::prev(leaderBoard.end()));
    }
}