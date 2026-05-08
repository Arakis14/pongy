#include <print>
#include <SDL3/SDL.h>

int main()
{
    std::println("Hello");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 0;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Hello SDL3",
        800,
        600,
        0
    );

    if (window == NULL) {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Delay(5000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}