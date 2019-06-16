#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <string.h>


class Character {
public:
    Character() {}

    ~Character() {}

    void init(SDL_Renderer *renderer, std::string path, const int width, const int height) {
        // Load the images

        // The surface is only useful until SDL_CreateTextureFromSurface is called.
        SDL_Surface *tempSurface = IMG_Load(path.c_str());
        playerTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        // Define the player image render size.
        this->width = width;
        this->height = height;
    }

    void updatePos(const int x, const int y) {
        this->x = x;
        this->y = y;
    }

    void render(SDL_Renderer *renderer) {
        SDL_Rect posRect = {this->x, this->y, this->width, this->height};
        // show the player image.
        SDL_RenderCopy(renderer, playerTexture, nullptr, &posRect);
    }

    void clean() {
        SDL_DestroyTexture(playerTexture);
    }

private:
    SDL_Texture *playerTexture;

    int width;
    int height;
    int x;
    int y;
};