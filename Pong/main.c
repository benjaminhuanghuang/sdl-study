#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define BALL_SIZE 20
#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 75
#define PLAYER_MARGIN 10
#define PLAYER_SPEED 150

typedef struct
{
    float x, y;
    float xSpeed, ySpeed;
    int size;
} Ball;

Ball ball;

typedef struct
{
    float x, y;
    int score;
} Player;
Player player1;
Player player2;

bool served = false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool Initialize();
void Update(float elapsed);
void Shutdown();

Ball MakeBall(int size);
void UpdateBall(Ball *ball, float elapsed);
void RenderBall(Ball *ball);
bool CoinFlip();

Player MakePlayer();
void UpdatePlayer(Player *player, float elapsed);
void RenderPlayer();

int main(int argc, char *argv[])
{
    srand(time(NULL));
    atexit(Shutdown);

    if (!Initialize())
    {
        exit(1);
    }

    bool quit = false;
    SDL_Event event;
    Uint32 lastTick = SDL_GetTicks(); // number of milliseconds

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_UP:
                    break;
                case SDLK_DOWN:
                    break;
                case SDLK_LEFT:
                    break;
                case SDLK_RIGHT:
                    break;
                }
                break;
            case SDL_KEYUP:
                break;
            default:
                break;
            }
        }
        Uint32 tick = SDL_GetTicks();
        float elapsed = (tick - lastTick) / 1000.0f;
        Update(elapsed);
        lastTick = tick;
    }

    Shutdown();
    return 0;
}
bool Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "ERROR: SDL_Init");
        return false;
    }

    window = SDL_CreateWindow("Pong Game", 100, 100, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "ERROR: !window");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "ERROR: !renderer");
        return false;
    }

    ball = MakeBall(BALL_SIZE);
    player1 = MakePlayer();
    player2 = MakePlayer();

    return true;
}

void Update(float elapsed)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    UpdateBall(&ball, elapsed);
    RenderBall(&ball);

    UpdatePlayer(&player1, elapsed);
    RenderPlayer(&player1);

    UpdatePlayer(&player2, elapsed);
    RenderPlayer(&player2);

    SDL_RenderPresent(renderer);
}

void Shutdown()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

Ball MakeBall(int size)
{
    Ball ball;
    ball.x = WIN_WIDTH / 2 - size / 2;
    ball.y = WIN_HEIGHT / 2 - size / 2;
    ball.xSpeed = 100 * (CoinFlip() ? 1 : -1);
    ball.ySpeed = 100 * (CoinFlip() ? 1 : -1);
    ball.size = size;
    return ball;
}

void UpdateBall(Ball *ball, float elapsed)
{
    ball->x += ball->xSpeed * elapsed;
    ball->y += ball->ySpeed * elapsed;

    if (ball->x < BALL_SIZE / 2)
    {
        ball->x = BALL_SIZE / 2;
        ball->xSpeed = -ball->xSpeed;
    }
    if (ball->x + BALL_SIZE / 2 > WIN_WIDTH)
    {
        ball->x = WIN_WIDTH - BALL_SIZE / 2;
        ball->xSpeed = -ball->xSpeed;
    }
    if (ball->y < BALL_SIZE / 2)
    {
        ball->y = BALL_SIZE / 2;
        ball->ySpeed = -ball->ySpeed;
    }
    if (ball->y + BALL_SIZE / 2 > WIN_HEIGHT)
    {
        ball->y = WIN_HEIGHT - BALL_SIZE / 2;
        ball->ySpeed = -ball->ySpeed;
    }
    // printf("elapsed: %f, x: %f, y: %f, xSpeed: %f, ySpeed: %f\n", elapsed, ball->x, ball->y, ball->xSpeed, ball->ySpeed);
}

void RenderBall(Ball *ball)
{
    int size = ball->size;
    int halfSize = size / 2;
    SDL_Rect ballRect = {
        .x = ball->x - halfSize,
        .y = ball->y - halfSize,
        .w = size,
        .h = size,
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ballRect);
}

bool CoinFlip()
{
    return rand() % 2 == 1;
}

Player MakePlayer()
{
    Player player;
    player.y = WIN_HEIGHT / 2;
    return player;
}

void UpdatePlayer(Player *player, float elapsed)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_SPACE])
    {
        served = true;
    }
    if (state[SDL_SCANCODE_W])
    {
        player1.y -= PLAYER_SPEED * elapsed;
    }
    if (state[SDL_SCANCODE_S])
    {
        player1.y += PLAYER_SPEED * elapsed;
    }
    if (state[SDL_SCANCODE_UP])
    {
        player2.y -= PLAYER_SPEED * elapsed;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        player2.y += PLAYER_SPEED * elapsed;
    }
}

void RenderPlayer()
{
    // Player1
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect playerRect = {
        .x = PLAYER_MARGIN,
        .y = player1.y - PLAYER_HEIGHT / 2,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &playerRect);

    // Player2
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect player2Rect = {
        .x = WIN_WIDTH - PLAYER_MARGIN - PLAYER_WIDTH,
        .y = player2.y - PLAYER_HEIGHT / 2,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &player2Rect);
}
