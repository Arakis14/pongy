#include <print>
#include <SDL3/SDL.h>

int main()
{
    std::println("Hello");
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Hello SDL3",
        800,
        600,
        0
    );

    SDL_Delay(2000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}