#include "SceneMain.h"
#include <iostream>
#include <SDL_image.h>
#include "Game.h"

SceneMain::SceneMain() : game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::init()
{
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;
}

void SceneMain::update()
{
    keyboardControl();
}

void SceneMain::render()
{
    SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);
}
void SceneMain::clean()
{
    SDL_DestroyTexture(player.texture);
}

void SceneMain::handleEvent(SDL_Event *event)
{
}

void SceneMain::keyboardControl()
{
    auto keyboardState = SDL_GetKeyboardState(nullptr);

    if (keyboardState[SDL_SCANCODE_W])
    {
        player.position.y -= 5;
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        player.position.y += 5;
    }
    if (keyboardState[SDL_SCANCODE_A])
    {
        player.position.x -= 5;
    }
    if (keyboardState[SDL_SCANCODE_D])
    {
        player.position.x += 5;
    }

    if (player.position.x < 0)
    {
        player.position.x = 0;
    }
    if (player.position.x > game.getWindowWidth() - player.width)
    {
        player.position.x = game.getWindowWidth() - player.width;
    }
    if (player.position.y < 0)
    {
        player.position.y = 0;
    }
    if (player.position.y > game.getWindowHeight() - player.height)
    {
        player.position.y = game.getWindowHeight() - player.height;
    }
}