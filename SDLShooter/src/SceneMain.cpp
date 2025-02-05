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
    std::random_device rd;
    randomEngine = std::mt19937(rd());
    dist = std::uniform_real_distribution(0.0f, 1.0f);

    // Load player
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;

    // Init templates
    // Template for player projectile
    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
    SDL_QueryTexture(projectilePlayerTemplate.texture, nullptr, nullptr, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    projectilePlayerTemplate.width /= 4;
    projectilePlayerTemplate.height /= 4;

    // Template for enemy
    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-2.png");
    SDL_QueryTexture(enemyTemplate.texture, nullptr, nullptr, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width /= 4;
    enemyTemplate.height /= 4;

    // Template for enemy projectile
    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-1.png");
    SDL_QueryTexture(projectileEnemyTemplate.texture, nullptr, nullptr, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    projectileEnemyTemplate.width /= 4;
    projectileEnemyTemplate.height /= 4;
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updatePlayerProjectiles(deltaTime);
    updateEnemyProjectiles(deltaTime);
    spawnEnemy();
    updateEnemies(deltaTime);
}

void SceneMain::render()
{
    // Render player
    SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);

    renderPlayerProjectiles();

    // Render enemies
    for (auto enemy : enemies)
    {
        SDL_Rect enemyRect = {static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height};
        SDL_RenderCopy(game.getRenderer(), enemy->texture, nullptr, &enemyRect);
    }
    // Render enemy projectiles
    renderEnemyProjectiles();
}
void SceneMain::clean()
{
    // clean bullets
    for (auto &projectile : projectilesPlayer)
    {
        if (projectile != nullptr)
        {
            delete projectile;
        }
    }
    projectilesPlayer.clear();

    // Clean enemies
    for (auto &enemy : enemies)
    {
        if (enemy != nullptr)
        {
            delete enemy;
        }
    }
    enemies.clear();

    // Clean enemy bullets
    for (auto &projectile : projectilesEnemy)
    {
        if (projectile != nullptr)
        {
            delete projectile;
        }
    }
    projectilesEnemy.clear();

    // Clean  the texture
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
    }
    if (projectilePlayerTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }
    if (enemyTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(enemyTemplate.texture);
    }
    if (projectileEnemyTemplate.texture)
    {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
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

void SceneMain::spawnEnemy()
{
    if (dist(randomEngine) > 1 / 60.f)
    {
        return;
    }

    Enemy *enemy = new Enemy(enemyTemplate);
    enemy->position.x = dist(randomEngine) * (game.getWindowWidth() - enemy->width);
    enemy->position.y = -enemy->height;

    enemies.push_back(enemy);
}

void SceneMain::updateEnemies(float deltaTime)
{
    auto currTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        auto enemy = *it;
        enemy->position.y += deltaTime * enemy->speed;
        if (enemy->position.y > game.getWindowHeight())
        {
            delete enemy;
            it = enemies.erase(it);
            SDL_Log("Enemy deleted");
        }
        else
        {
            // Enemy fire
            if (currTime - enemy->lastShotTime > enemy->coolDown)
            {
                shootEnemy(enemy);
                enemy->lastShotTime = currTime;
            }
            ++it;
        }
    }
}

void SceneMain::shootEnemy(Enemy *enemy)
{
    auto *projectile = new ProjectileEnemy(projectileEnemyTemplate);
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;
    projectile->direction = getDirection(enemy);
    projectilesEnemy.push_back(projectile);
}

SDL_FPoint SceneMain::getDirection(Enemy *enemy)
{
    auto x = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
    auto y = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);
    auto length = sqrt(x * x + y * y);
    x /= length;
    y /= length;
    return SDL_FPoint{x, y};
}

void SceneMain::updateEnemyProjectiles(float deltaTime)
{
    int margin = 32;
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();)
    {
        auto projectile = *it;
        projectile->position.x += deltaTime * projectile->speed * projectile->direction.x;
        projectile->position.y += deltaTime * projectile->speed * projectile->direction.y;
        if (projectile->position.y + margin < 0 ||
            projectile->position.y - margin > game.getWindowHeight() ||
            projectile->position.x + margin < 0 ||
            projectile->position.x - margin > game.getWindowWidth())
        {
            delete projectile;
            it = projectilesEnemy.erase(it);
            SDL_Log("Enemy projectile deleted");
        }
        else
        {
            ++it;
        }
    }
}

void SceneMain::renderEnemyProjectiles()
{
    for (auto projectile : projectilesEnemy)
    {
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height};
        SDL_RenderCopy(game.getRenderer(), projectile->texture, nullptr, &projectileRect);
    }
}