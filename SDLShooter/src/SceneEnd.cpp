#include "SceneEnd.h"
#include "Game.h"
#include "SceneMain.h"
#include <string>

void SceneEnd::init()
{
    // start input
    if (!SDL_IsTextInputActive())
    {
        SDL_StartTextInput();
    }
}

void SceneEnd::update(float deltaTime)
{
    blinkTimer -= deltaTime;
    if (blinkTimer < 0)
    {
        blinkTimer += 1.0f;
    }
}
void SceneEnd::render()
{
    if (isTyping)
    {
        renderPhase1();
    }
    else
    {
        renderPhase2();
    }
}

void SceneEnd::clean()
{
}

void SceneEnd::handleEvent(SDL_Event *event)
{
    if (isTyping)
    {
        if (event->type == SDL_TEXTINPUT)
        {
            playerName += event->text.text;
        }

        if (event->type == SDL_KEYDOWN)
        {
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN)
            {
                isTyping = false;
                SDL_StopTextInput();
                if (playerName == "")
                {
                    playerName = "Anon-Player";
                }
            }
            else if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
            {
                removeLastUTF8Char(playerName);
            }
        }
    }
    else
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
}

void SceneEnd::renderPhase1()
{
    auto score = game.getFinalScore();
    std::string scoreText = "Your score: " + std::to_string(score);
    std::string gameOver = "Game Over";
    std::string instructionText = "Press any key to continue";

    game.renderTextCentered(gameOver, 0.4, true);
    game.renderTextCentered(scoreText, 0.2, false);
    game.renderTextCentered(instructionText, 0.6, false);

    if (playerName != "")
    {
        SDL_Point p = game.renderTextCentered(playerName, 0.8, false);
        if (blinkTimer < 0.5f)
        {
            game.renderTextPos("_", p.x, p.y);
        }
    }
    else
    {
        if (blinkTimer < 0.5f)
        {
            game.renderTextCentered("_", 0.8, false);
        }
    }
}
void SceneEnd::renderPhase2()
{
    game.renderTextCentered("Leaderboard", 0.1, true);
    int i = 1;
    auto posY = 0.2 * game.getWindowHeight();
    for (auto item : game.getLeaderBoard())
    {
        std::string name = std::to_string(i) + ". " + item.second;
        std::string score = std::to_string(item.first);
        game.renderTextPos(name, 100, posY);
        game.renderTextPos(score, 100, posY, false);
        posY += 50;
    }

    game.renderTextCentered("Press J back to new game", 0.9, false);
}

void SceneEnd::removeLastUTF8Char(std::string &str)
{
    if (str.empty())
    {
        return;
    }
    auto lastchar = str.back();
    if ((lastchar & 0b10000000) == 0b10000000) // is last part of chinese
    {
        str.pop_back();
        while ((str.back() & 0b11000000) != 0b11000000) // remove all chinese parts utile the first part
        {
            str.pop_back();
        }
    }
    str.pop_back();
}