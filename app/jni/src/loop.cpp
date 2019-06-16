#include <iostream>

#include <SDL.h>
#include "Game.cpp"

#define SDL_MAIN_HANDLED

const bool IS_MOBILE = true;

static const int TARGET_FPS = 60;
// Max time we want to have between frames (in ms).
static const int MAX_FRAME_TIME = 1000 / TARGET_FPS;

int main(int argc, char *argv[]) {
    Game game = Game();

    game.init("Super game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );

    uint32_t tickStart;
    int currentFrameTime;

    while(game.running()) {
        // Ticks since we've first initialized the SDL for FPS.
        tickStart = SDL_GetTicks();

        game.handleEvents();
        game.update();
        game.render();

        // Compute how long it took to render the frame.
        currentFrameTime = SDL_GetTicks() - tickStart;

        // If we still have time, delay the next render.
        if (MAX_FRAME_TIME > currentFrameTime) {
            SDL_Delay(MAX_FRAME_TIME - currentFrameTime);
        }
    }

    game.clean();

    return 0;
}

// #include <android/log.h>
// __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%f", event.tfinger.x);