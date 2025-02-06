#include "SceneEnd.h"
#include "Game.h"
#include <string>

void SceneEnd::init()
{
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
}
void SceneEnd::renderPhase2()
{
}