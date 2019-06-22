#ifndef PP_CPP
#define PP_CPP

#include <functional>

#include <SDL.h>
#include <SDL_image.h>

class Game;  // Cyclic import.

class PP {
public:
    static PP &getInstance() {
        static PP instance; // Guaranteed to be destroyed. Instantiated only on the first use.
        return instance;
    }

    // C++ 11
    // Delete the methods we don't want as it's a singleton.
    PP(PP const &) = delete;
    void operator=(PP const &) = delete;

    int init(const char *title, int xpos, int ypos) {
        if (this->is_init)
            return -1;

        is_init = true;

        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            SDL_Log("SDL could not initialize: %s\n", SDL_GetError());
            return false;
        }

        this->window = SDL_CreateWindow(title, xpos, ypos, 360, 640, SDL_WINDOW_ALLOW_HIGHDPI);
        if (this->window == nullptr) {
            SDL_Log("Could not create window: %s\n", SDL_GetError());
            return false;
        }

        this->renderer = SDL_CreateRenderer(window, -1, 0);
        if (this->renderer == nullptr) {
            SDL_Log("Could not create renderer: %s\n", SDL_GetError());
            return false;
        }

        // Get the actual width and height of the screen.
        SDL_GetWindowSize(this->window, &this->screen_width, &this->screen_height);
        std::cout << "Window size: " << this->screen_width << " per " << this->screen_height << std::endl;

        return true;
    }

    bool getEvents(SDL_Event *event) {
        return SDL_PollEvent(event);
    }

    /**
    * Render the SDL and a game object.
    */
    void render(Game* game, std::function<void(Game*, SDL_Renderer*)> func) {
        // Set the default color of the screen before the clear.
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        // Clear the screen.
        SDL_RenderClear(renderer);

        func(game, this->renderer);

        // Update the screen.
        SDL_RenderPresent(renderer);
    }

    /**
     * Render a rectangle, but with an intuitive position (as SDL set 0, 0 to top left.)
     *
     * @param renderer
     * @param r
     * @param g
     * @param b
     * @param a
     * @param x
     * @param y
     * @param w
     * @param h
     */
    void render_pixel(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int x, int y, int w, int h) {
        // Draw forms.
        SDL_SetRenderDrawColor(renderer, r, g, b, a);

        SDL_Rect fillRect = {screen_width - x, screen_height - y, w, h};
        SDL_RenderFillRect(renderer, &fillRect);
    }

    /**
    * Clean the PP and any objects.
    */
    void clean() {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);

        SDL_Quit();
    }

private:
    bool is_init = false;

    int screen_width{};
    int screen_height{};

    SDL_Window *window{};
    SDL_Renderer *renderer{};

    /** Constructor is private as this is a singleton. */
    PP() {}
};

#endif // PP_CPP