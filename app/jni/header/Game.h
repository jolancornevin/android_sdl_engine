#ifndef SDL_INTRO_GAME_H
#define SDL_INTRO_GAME_H


#include "SDL.h"

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


#endif //SDL_INTRO_GAME_H
