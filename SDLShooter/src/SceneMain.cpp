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
    // Load player
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;

    // Init template
    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
    SDL_QueryTexture(projectilePlayerTemplate.texture, nullptr, nullptr, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    projectilePlayerTemplate.width /= 4;
    projectilePlayerTemplate.height /= 4;
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updatePlayerProjectiles(deltaTime);
}

void SceneMain::render()
{
    // Render player
    SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);

    renderPlayerProjectiles();
}
void SceneMain::clean()
{
    for (auto &projectile : projectilesPlayer)
    {
        if (projectile != nullptr)
        {
            delete projectile;
        }
    }
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
    }
    if (projectilePlayerTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }
}

void SceneMain::handleEvent(SDL_Event *event)
{
}

void SceneMain::keyboardControl(float deltaTime)
{
    auto keyboardState = SDL_GetKeyboardState(nullptr);

    if (keyboardState[SDL_SCANCODE_W])
    {
        player.position.y -= deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        player.position.y += deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_A])
    {
        player.position.x -= deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_D])
    {
        player.position.x += deltaTime * player.speed;
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

    // Fire
    if (keyboardState[SDL_SCANCODE_J])
    {
        auto currTime = SDL_GetTicks();
        if (currTime - player.lastShotTime > player.coolDown)
        {
            shoot();
            player.lastShotTime = currTime;
        }
    }
}

void SceneMain::shoot()
{
    auto *projectile = new ProjectilePlayer(projectilePlayerTemplate);
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y - projectile->height;
    projectilesPlayer.push_back(projectile);
}

void SceneMain::updatePlayerProjectiles(float deltaTime)
{
    int margin = 32;
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();)
    {
        auto projectile = *it;
        projectile->position.y -= deltaTime * projectile->speed;
        if (projectile->position.y + margin < 0)
        {
            delete projectile;
            it = projectilesPlayer.erase(it);
            SDL_Log("Player projectile deleted");
        }
        else
        {
            ++it;
        }
    }
}

void SceneMain::renderPlayerProjectiles()
{
    for (auto projectile : projectilesPlayer)
    {
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height};
        SDL_RenderCopy(game.getRenderer(), projectile->texture, nullptr, &projectileRect);
    }
}