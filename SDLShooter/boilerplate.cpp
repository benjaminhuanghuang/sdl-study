#include <iostream> 
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

int main(int, char**) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL Shooter", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Load image
    if( IMG_Init(IMG_INIT_PNG| IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG) ) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture* texture = IMG_LoadTexture(renderer, "assets/image/bg.png");

    // Audio
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        SDL_DestroyTexture(texture);
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }   
    Mix_Music* music = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if (music == nullptr) {
        std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
        Mix_CloseAudio();
        SDL_DestroyTexture(texture);
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    Mix_PlayMusic(music, -1);

    // TTF
    if(TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        Mix_Quit();
        SDL_DestroyTexture(texture);
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    TTF_Font* font = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 24);
    SDL_Color color = {255, 255, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, "Hello, SDL!", color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, surface);

    while (true) {
        SDL_Event e;
        if(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
               break;
            }
        }
        // Clear screen
        SDL_RenderClear(renderer);
        // Draw picture
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        // Draw text
        SDL_Rect textRect = {10, 10, surface->w, surface->h};
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean image resources
    SDL_DestroyTexture(texture);
    IMG_Quit();

    // Clean audio resources
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();
    
    // Clean TTF resources
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}