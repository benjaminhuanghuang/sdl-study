#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"
#include <list>
#include <map>
#include <random>
#include <SDL.h>
#include <SDL_mixer.h>

class Game;

class SceneMain : public Scene
{
public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    void handleEvent(SDL_Event *event) override;
    void keyboardControl(float deltaTime);
    void shoot();
    void updatePlayerProjectiles(float deltaTime);
    void renderPlayerProjectiles();
    void spawnEnemy();
    void updatePlayer(float deltaTime);
    void updateEnemyProjectiles(float deltaTime);
    void updateExplosions(float deltaTime);
    void renderEnemyProjectiles();
    void updateEnemies(float deltaTime);
    void updateItems(float deltaTime);
    void shootEnemy(Enemy *enemy);
    SDL_FPoint getDirection(Enemy *enemy);
    void enemyExplode(Enemy *enemy);
    void renderExplosions();
    void dropItem(Enemy *enemy);
    void playerGetItem(Item *item);
    void renderItems();
    void renderUI();

private:
    Game &game;
    Player player;
    Mix_Music *bgm;
    SDL_Texture *uiHealth;

    bool isDead = false;
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> dist;
    // Object template
    ProjectilePlayer projectilePlayerTemplate;
    Enemy enemyTemplate;
    ProjectileEnemy projectileEnemyTemplate;
    Explosion explosionTemplate;
    Item itemLifeTemplate;

    std::list<ProjectilePlayer *> projectilesPlayer;
    std::list<Enemy *> enemies;
    std::list<ProjectileEnemy *> projectilesEnemy;
    std::list<Explosion *> explosions;
    std::list<Item *> items;
    std::map<std::string, Mix_Chunk *> sounds;
};
#endif // SCENE_MAIN_H