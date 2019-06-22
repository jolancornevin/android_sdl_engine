#ifndef GAME_CPP
#define GAME_CPP

#include <stdio.h>
#include <string>
#include <string.h>

#include <SDL.h>
#include <SDL_image.h>

#include "Character.cpp"
#include "utils/PP.cpp"
#include "phygine/Fireworks.cpp"

extern const bool IS_MOBILE;

class Game {
public:
    Game() = default;
    ~Game() = default;

    int init(const char *title, int xpos, int ypos) {
        isRunning = true;

        PP &pp = PP::getInstance();
        pp.init("Super game", xpos, ypos);

        return EXIT_SUCCESS;
    }

    /**
    * Handle any event that might occurs in the application.
    */
    void handleEvents() {
        SDL_Event event;
        PP &pp = PP::getInstance();

        // Multiples event can have occurred since the last call. We de-pile them all with the while
        while (pp.getEvents(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_FINGERDOWN:
                    // event.tfinger.x and y are normalized, so we have to multiple them by the screen size to get the real pos.
                    this->c.updatePos(
                            event.tfinger.x * this->width,
                            event.tfinger.y * this->height);
                    break;
                case SDL_FINGERMOTION:
                    this->c.updatePos(
                            event.tfinger.x * this->width,
                            event.tfinger.y * this->height);
                    break;
                case SDL_FINGERUP:
                    break;
                default:
                    break;
            }
        }
    }

    /**
     * Update the elment of the games.
     *
     * @param lastFrameDuration: time elapsed in ms.
     */
    void update(float lastFrameDuration) {
        this->fireworkHandler.update(lastFrameDuration);
    }

    /**
    * Render the game and every objects in it.
    */
    void _render(SDL_Renderer* renderer) {
        // this->c.render(renderer);
        this->fireworkHandler.display(renderer);
    }

    void render() {
        PP &pp = PP::getInstance();
        pp.render(this, &Game::_render);
    }

    void clean() {
        // this->c.clean();
        PP &pp = PP::getInstance();
        pp.clean();
    }

    bool running() { return isRunning; };

private:
    bool isRunning{};
    FireworksDemo fireworkHandler;
};

#endif // GAME_CPP