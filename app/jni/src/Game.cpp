//
// Created by hp on 14/06/2019.
//

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <string.h>


class Game {
public:
    Game();
    ~Game();

    int init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() { return isRunning; };
private:
    bool isRunning;

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *playerTexture;
};


// Don't work if it's at the class level ?
SDL_Rect dstrect;

Game::Game() {}

Game::~Game() {}

int Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen) {
    isRunning = true;

    SDL_Surface *imageSurface = nullptr;
    SDL_Surface *windowSurface = nullptr;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        isRunning = false;
        return -1;
    }

    window = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == nullptr) {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        isRunning = false;
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
        isRunning = false;
        return -1;
    }

    // Load the images

    // The surface is only useful until SDL_CreateTextureFromSurface is called.
    SDL_Surface *tempSurface = IMG_Load("main/hello_world.bmp");
    playerTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // Define the player image render size.
    dstrect.h = 32;
    dstrect.w = 32;

    return EXIT_SUCCESS;
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            break;
    }
}

void Game::update() {}

void Game::render() {
    SDL_RenderClear(renderer);

    // show the player image.
    SDL_RenderCopy(renderer, playerTexture, NULL, &dstrect);

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(playerTexture);

    SDL_Quit();
}