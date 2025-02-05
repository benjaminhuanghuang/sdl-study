#include "SceneTitle.h"
#include <string>
#include "Game.h"
#include "SceneMain.h"

void SceneTitle::init()
{
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    Mix_PlayMusic(bgm, -1);
}

void SceneTitle::update(float deltaTime)
{
}

void SceneTitle::render()
{
    std::string title = "SDL Shooter";
    game.renderTextCentered(title, 0.2f, true);

    std::string instruction = "Press J to start";
    game.renderTextCentered(instruction, 0.6f, false);
}

void SceneTitle::clean()
{
    if (bgm != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void SceneTitle::handleEvent(SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.scancode == SDL_SCANCODE_J)
        {
            auto sceneMain = new SceneMain();
            game.changeScene(sceneMain);
        }
    }
}