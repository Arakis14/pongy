#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdint>
#include <cstdlib>

const char* brickBG = "../assets/brick.bmp";
//userinput speed?
const uint8_t speedMulti = 1;
double deltaTime = 0;
const uint16_t resolutionHeight = 1024;
const uint16_t resolutionWidth = 768;

double checkHeight(SDL_FRect& paddle)
{
    return paddle.y;
}

void  increaseHeight(SDL_FRect& paddle)
{
    paddle.y -= 2 * speedMulti * deltaTime;
    //DEBUG
    //SDL_Log("New paddle y: %f", paddle.y);
}

void  decreaseHeight(SDL_FRect& paddle)
{
    paddle.y += 2 * speedMulti * deltaTime;
    //DEBUG
    //SDL_Log("New paddle y: %f", paddle.y);
}

int main()
{
    SDL_Window *window;
    SDL_FRect paddle1 = {50, 50, 50, 100};
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", resolutionHeight, resolutionWidth, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    bool quit_the_app = false;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);
    while (!quit_the_app) {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            quit_the_app = true;
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
            /* the pressed key was Escape? */
            SDL_Log("Key %s pressed", SDL_GetKeyName(event.key.key));
            if (event.key.key == SDLK_ESCAPE) {
                quit_the_app = true;
            }
            else if(event.key.key == SDLK_DOWN || event.key.key == SDLK_S)
            {
                if(checkHeight(paddle1) < resolutionWidth - paddle1.h) {
                    decreaseHeight(paddle1);
                }
            }
            else if(event.key.key == SDLK_UP || event.key.key == SDLK_W)
            {
                if(checkHeight(paddle1) > 0.00) {
                    increaseHeight(paddle1);
                }
            }
        }
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderRect(renderer,&paddle1);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddle1);
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