#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdlib>
#include <ctime>
const char* brickBG = "../assets/brick.bmp";
int main()
{
    std::srand((unsigned int)std::time(nullptr));
    SDL_Window *window;
    SDL_FRect r;
    SDL_Renderer *renderer;
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

    bool quit_the_app = false;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);
    r.w = 50;
    r.h = 150;
    r.x = 50;
    r.y = 50;
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
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderRect(renderer,&r);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &r);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}