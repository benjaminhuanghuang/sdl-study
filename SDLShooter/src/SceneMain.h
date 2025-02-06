#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"
#include <list>
#include <map>
#include <random>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class SceneMain : public Scene
{
public:
    ~SceneMain();
    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    void handleEvent(SDL_Event *event) override;

private:
    Player player;
    Mix_Music *bgm;
    SDL_Texture *uiHealth;
    TTF_Font *scoreFont;
    int score = 0;
    float timerEnd = 0;

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

    // Update
    void updatePlayerProjectiles(float deltaTime);
    void updatePlayer(float deltaTime);
    void updateEnemyProjectiles(float deltaTime);
    void updateExplosions(float deltaTime);
    void updateEnemies(float deltaTime);
    void updateItems(float deltaTime);
    void spawnEnemy();
    void keyboardControl(float deltaTime);
    void changeSceneDelayed(float deltaTime, float delay);

    // Render
    void renderPlayerProjectiles();
    void renderExplosions();
    void renderEnemyProjectiles();
    void renderItems();
    void renderUI();
    // others
    void shoot();
    void shootEnemy(Enemy *enemy);
    SDL_FPoint getDirection(Enemy *enemy);
    void enemyExplode(Enemy *enemy);
    void dropItem(Enemy *enemy);
    void playerGetItem(Item *item);
};
#endif // SCENE_MAIN_H