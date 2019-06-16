#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <string>
#include <string.h>

#include "Character.cpp"

extern const bool IS_MOBILE;

class Game {
public:
    Game() = default;

    ~Game() = default;

    int init(const char *title, int xpos, int ypos, bool fullscreen) {
        isRunning = true;

        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        this->width = DM.w;
        this->height = DM.h;

        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
            isRunning = false;
            return -1;
        }

        window = SDL_CreateWindow(title, xpos, ypos, 360, 640, SDL_WINDOW_ALLOW_HIGHDPI);
        if (window == nullptr) {
            SDL_Log("Could not create window:  %s\n", SDL_GetError());
            isRunning = false;
            return -1;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == nullptr) {
            SDL_Log("Could not create renderer: %s\n", SDL_GetError());
            isRunning = false;
            return -1;
        }

        // Get the actual width and height of the screen.
        SDL_GetWindowSize(window, &this->width, &this->height);

        std::string path = "";
        if (IS_MOBILE) {
            path = "main/hello_world.bmp";
        } else {
            path = "D:\\Documents\\Dev\\Project\\sdl_intro\\main\\hello_world.bmp";
        }
        // Create a character.
        this->c.init(renderer, path, 256, 256);

        return EXIT_SUCCESS;
    }

    /**
    * Handle any event that might occurs in the application.
    */
    void handleEvents() {
        SDL_Event event;

        // Multiples event can have occurred since the last call. We de-pile them all with the while
        while (SDL_PollEvent(&event)) {
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

    void update() {}

    /**
    * Render the game and every objects in it.
    */
    void render() {
        SDL_RenderClear(renderer);

        this->c.render(renderer);

        SDL_RenderPresent(renderer);
    }

    /**
    * Clean the SDL and any objects.
    */
    void clean() {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);

        this->c.clean();

        SDL_Quit();
    }

    bool running() { return isRunning; };
private:
    bool isRunning{};

    SDL_Window *window{};
    SDL_Renderer *renderer{};
    SDL_Texture *playerTexture{};

    int width{};
    int height{};

    Character c;
};
