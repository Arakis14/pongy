#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdlib>
#include <ctime>
const char* brickBG = "../assets/brick.bmp";
int main()
{
    std::srand((unsigned int)std::time(nullptr));
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Event event;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", 1024, 768, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    surface = SDL_LoadBMP(brickBG);
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        return 3;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return 3;
    }
    SDL_DestroySurface(surface);

    Uint64 lastChange = SDL_GetTicks();
    Uint8 r = 0, g = 0, b = 0;
    bool quit_the_app = false;
    while (!quit_the_app) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            quit_the_app = true;
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
            /* the pressed key was Escape? */
            SDL_Log("Key pressed");
            if (event.key.key == SDLK_ESCAPE) {
                quit_the_app = true;
            }
        }
        Uint64 now = SDL_GetTicks();

        if (now - lastChange >= 2000)
        {
            r = rand() % 256;
            g = rand() % 256;
            b = rand() % 256;

            lastChange = now;
        }

        // Set draw color
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);

        // Clear screen with current color
        SDL_RenderClear(renderer);

        // Present result
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}