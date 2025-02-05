#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"
#include <list>
#include <random>

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
    void updateEnemies(float deltaTime);

private:
    Game &game;
    Player player;
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> dist;
    // Object template
    ProjectilePlayer projectilePlayerTemplate;
    Enemy enemyTemplate;

    std::list<ProjectilePlayer *> projectilesPlayer;
    std::list<Enemy *> enemies;
};
#endif // SCENE_MAIN_H