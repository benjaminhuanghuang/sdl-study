#include "SceneEnd.h"
#include "Game.h"
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
            }
            else if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
            {
                removeLastUTF8Char(playerName);
            }
        }
    }
    else
    {
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
        std::string playerNameText = "Player: " + playerName;
        game.renderTextCentered(playerNameText, 0.3, false);
    }
}
void SceneEnd::renderPhase2()
{
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