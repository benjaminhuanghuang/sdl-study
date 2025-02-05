#include "SceneMain.h"
#include <iostream>
#include <SDL_image.h>
#include "Game.h"
#include <SDL.h>

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

    // Template for explosion
    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/effect/explosion.png");
    SDL_QueryTexture(explosionTemplate.texture, nullptr, nullptr, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrame = explosionTemplate.width / explosionTemplate.height;
    explosionTemplate.width = explosionTemplate.height;

    // Template for item
    itemLifeTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bonus_life.png");
    SDL_QueryTexture(itemLifeTemplate.texture, nullptr, nullptr, &itemLifeTemplate.width, &itemLifeTemplate.height);
    itemLifeTemplate.width /= 4;
    itemLifeTemplate.height /= 4;
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updatePlayerProjectiles(deltaTime);
    updateEnemyProjectiles(deltaTime);
    spawnEnemy();
    updateEnemies(deltaTime);
    updatePlayer(deltaTime);
    updateExplosions(deltaTime);
}

void SceneMain::render()
{
    // Render player
    if (!isDead)
    {
        SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
        SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);
    }
    renderPlayerProjectiles();

    // Render enemies
    for (auto enemy : enemies)
    {
        SDL_Rect enemyRect = {static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height};
        SDL_RenderCopy(game.getRenderer(), enemy->texture, nullptr, &enemyRect);
    }
    // Render enemy projectiles
    renderEnemyProjectiles();
    renderItems();
    renderExplosions();
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

    // Clean explosions
    for (auto &explosion : explosions)
    {
        if (explosion != nullptr)
        {
            delete explosion;
        }
    }
    explosions.clear();
    // Clean items
    for (auto &item : items)
    {
        if (item != nullptr)
        {
            delete item;
        }
    }
    items.clear();

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
    if (explosionTemplate.texture)
    {
        SDL_DestroyTexture(explosionTemplate.texture);
    }
    if (itemLifeTemplate.texture)
    {
        SDL_DestroyTexture(itemLifeTemplate.texture);
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
            bool hit = false; // Determine if the bullet hit any enemy
            for (auto enemy : enemies)
            {
                SDL_Rect enemyRect = {
                    static_cast<int>(enemy->position.x),
                    static_cast<int>(enemy->position.y),
                    enemy->width,
                    enemy->height};

                SDL_Rect projectileRect = {
                    static_cast<int>(projectile->position.x),
                    static_cast<int>(projectile->position.y),
                    projectile->width,
                    projectile->height};
                if (SDL_HasIntersection(&enemyRect, &projectileRect))
                {
                    hit = true;
                    enemy->currentHealth -= projectile->damage;
                    delete projectile;
                    it = projectilesPlayer.erase(it);
                    SDL_Log("Enemy hit");
                    break;
                }
            }
            if (!hit)
            {
                ++it;
            }
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

void SceneMain::updatePlayer(float)
{
    if (isDead)
        return;

    if (player.currentHealth <= 0)
    {
        isDead = true;
        // Player is dead, create an explosion
        auto explosion = new Explosion(explosionTemplate);
        explosion->position.x = player.position.x + player.width / 2 - explosion->width / 2;
        explosion->position.y = player.position.y + player.height / 2 - explosion->height / 2;
        explosion->startTime = SDL_GetTicks();
        explosions.push_back(explosion);

        return;
    }
    for (auto enemy : enemies)
    {
        SDL_Rect playerRect = {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height};
        SDL_Rect enemyRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height};
        if (SDL_HasIntersection(&playerRect, &enemyRect))
        {
            player.currentHealth -= 1;
            enemy->currentHealth = 0;
        }
    }
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
            if (currTime - enemy->lastShotTime > enemy->coolDown && !isDead)
            {
                shootEnemy(enemy);
                enemy->lastShotTime = currTime;
            }
            if (enemy->currentHealth <= 0)
            {
                enemyExplode(enemy);
                it = enemies.erase(it);
            }
            else
            {
                ++it;
            }
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
            SDL_Rect playerRect = {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height};
            SDL_Rect projectileRect = {
                static_cast<int>(projectile->position.x),
                static_cast<int>(projectile->position.y),
                projectile->width,
                projectile->height};
            if (SDL_HasIntersection(&playerRect, &projectileRect) && !isDead)
            {
                // Player is hit, remove the enemy bullet
                player.currentHealth -= projectile->damage;
                delete projectile;
                it = projectilesEnemy.erase(it);
            }
            else
            {
                ++it;
            }
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

        float angle = atan2(projectile->direction.y, projectile->direction.x) * 180 / M_PI - 90;
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, nullptr, &projectileRect, angle, nullptr, SDL_FLIP_NONE);
    }
}

void SceneMain::enemyExplode(Enemy *enemy)
{
    auto *explosion = new Explosion(explosionTemplate);
    explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2;
    explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->height / 2;
    explosion->startTime = SDL_GetTicks();
    explosions.push_back(explosion);

    if (dist(randomEngine) < 0.3f)
    {
        dropItem(enemy);
    }

    delete enemy;
}

void SceneMain::updateExplosions(float)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = explosions.begin(); it != explosions.end();)
    {
        auto explosion = *it;
        explosion->currentFrame = (currentTime - explosion->startTime) * explosion->FPS / 1000;
        if (explosion->currentFrame >= explosion->totalFrame)
        {
            delete explosion;
            it = explosions.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void SceneMain::renderExplosions()
{
    for (auto explosion : explosions)
    {
        SDL_Rect src = {
            explosion->currentFrame * explosion->width,
            0,
            explosion->width,
            explosion->height};

        SDL_Rect dest = {
            static_cast<int>(explosion->position.x),
            static_cast<int>(explosion->position.y),
            explosion->width,
            explosion->height};
        SDL_RenderCopy(game.getRenderer(), explosion->texture, &src, &dest);
    }
}

void SceneMain::dropItem(Enemy *enemy)
{
    auto *item = new Item(itemLifeTemplate);
    item->position.x = enemy->position.x + enemy->width / 2 - item->width / 2;
    item->position.y = enemy->position.y + enemy->height / 2 - item->height / 2;
    float angle = dist(randomEngine) * 2 * M_PI;
    item->direction = {cos(angle), sin(angle)};
    items.push_back(item);
}

void SceneMain::updateItems(float deltaTime)
{
    for (auto it = items.begin(); it != items.end();)
    {
        auto item = *it;
        // Update item position
        item->position.x += item->direction.x * item->speed * deltaTime;
        item->position.y += item->direction.y * item->speed * deltaTime;
        // Bounce back when hit the wall
        if (item->bounceCount > 0)
        {
            if (item->position.x < 0 || item->position.x > game.getWindowWidth() - item->width)
            {
                item->direction.x *= -1;
                item->bounceCount--;
            }
            if (item->position.y < 0 || item->position.y > game.getWindowHeight() - item->height)
            {
                item->direction.y *= -1;
                item->bounceCount--;
            }
        }
        // Delete item when out of screen
        if (item->position.x + item->width < 0 ||
            item->position.x > game.getWindowWidth() ||
            item->position.y + item->height < 0 ||
            item->position.y > game.getWindowHeight())
        {
            delete item;
            it = items.erase(it);
        }
        else
        {
            SDL_Rect playerRect = {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height};
            SDL_Rect itemRect = {
                static_cast<int>(item->position.x),
                static_cast<int>(item->position.y),
                item->width,
                item->height};
            if (SDL_HasIntersection(&playerRect, &itemRect))
            {
                playerGetItem(item);
                delete item;
                it = items.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void SceneMain::playerGetItem(Item *item)
{
    if (item->type == ItemType::Life)
    {
        player.currentHealth += 1;
        if (player.currentHealth > player.maxHealth)
        {
            player.currentHealth = player.maxHealth;
        }
    }
}

void SceneMain::renderItems()
{
    for (auto item : items)
    {
        SDL_Rect itemRect = {
            static_cast<int>(item->position.x),
            static_cast<int>(item->position.y),
            item->width,
            item->height};
        SDL_RenderCopy(game.getRenderer(), item->texture, nullptr, &itemRect);
    }
}