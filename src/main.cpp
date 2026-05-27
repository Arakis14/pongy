#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <cstdlib>
#include "Text.hpp"

//userinput speed?
const uint8_t speedMulti = 2;
double deltaTime = 0;
const uint16_t resolutionWidth = 1024;
const uint16_t resolutionHeight = 768;
uint8_t scorePlayer1 = 0;
uint8_t scorePlayer2 = 0;

// (1, 0) represents the direction to the right, (0, 1) represents the direction down
struct Vec2
{
    float x = 0;
    float y = 0;
};

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

void resetBall(SDL_FRect& ball)
{
    ball.x = 512;
    ball.y = 360;
}

void changeBallPosition(SDL_FRect& ball, Vec2& velocity, SDL_FRect& paddleLeft, SDL_FRect& paddleRight)
{
    ball.x += velocity.x * deltaTime;
    ball.y += velocity.y * deltaTime;

    // Top wall
    if (ball.y <= 0)
    {
        ball.y = ball.h;
        velocity.y *= -1;
    }

    // Bottom wall
    if (ball.y + ball.h >= resolutionHeight )
    {
        ball.y = resolutionHeight - ball.h;
        velocity.y *= -1;
    }
    // Left wall
    if (ball.x + ball.w >= resolutionWidth) 
    {
        scorePlayer2++;
        resetBall(ball);
    }
    //Right wall
    if (ball.x <= 0) 
    {
        scorePlayer1++;
        resetBall(ball);
    }

    if (SDL_HasRectIntersectionFloat(&ball, &paddleLeft))
    {
        velocity.x = std::abs(velocity.x);
        ball.x = paddleLeft.x + paddleLeft.w;
    }

    if (SDL_HasRectIntersectionFloat(&ball, &paddleRight))
    {
        velocity.x = -std::abs(velocity.x);
        ball.x = paddleRight.x - ball.w;
    }

}

bool checkIfGameOver()
{
    return (scorePlayer1 >=3 || scorePlayer2 >= 3);
}

int main()
{
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Window *window;
    SDL_FRect paddleLeft = {50, 50, 50, 100};
    SDL_FRect paddleRight = {924, 50, 50, 100};
    SDL_FRect ball = {512, 360, 15, 15};
    //Note: make rand starting velocity
    //float randomSeed = SDL_randf();
    Vec2 ballVelocity = {0.1, 0.1};
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;
    Text TextExample{"Hello World"};
    SDL_Surface* textSurface = TTF_RenderText_Blended(TextExample.getFont(), TextExample.getText().c_str(), 0, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer,textSurface);
    SDL_FRect dstRect{
        100.0f,
        100.0f,
        static_cast<float>(textSurface->w),
        static_cast<float>(textSurface->h)
    };

    SDL_DestroySurface(textSurface);
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    TTF_Init();
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", resolutionWidth, resolutionHeight, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
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
            else if(event.key.key == SDLK_DOWN) {
                if(checkHeight(paddleRight) < resolutionHeight - paddleRight.h) {
                    decreaseHeight(paddleRight);
                }
            }
            else if(event.key.key == SDLK_UP) {
                if(checkHeight(paddleRight) > 0.00) {
                    increaseHeight(paddleRight);
                }
            }
            else if (event.key.key == SDLK_S) {
                if(checkHeight(paddleLeft) < resolutionHeight - paddleLeft.h) {
                    decreaseHeight(paddleLeft);
                }
            }
            else if (event.key.key == SDLK_W) {
                if(checkHeight(paddleLeft) > 0.00) {
                    increaseHeight(paddleLeft);
                }
            }
        }
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        //render left paddle
        SDL_RenderRect(renderer,&paddleLeft);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddleLeft);
        //render right paddle
        SDL_RenderRect(renderer,&paddleRight);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddleRight);
        //render ball
        SDL_RenderRect(renderer,&ball);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &ball);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_RenderTexture(renderer, textTexture, nullptr, &dstRect);
        //loop
        changeBallPosition(ball, ballVelocity, paddleLeft, paddleRight);
        if (checkIfGameOver()) {
            break;
        }
        //DEBUG
        SDL_Log("paddleRight.x %f", paddleRight.x);
        SDL_Log("paddleRight.y %f", paddleRight.y);
        SDL_Log("ball.x %f", ball.x);
        SDL_Log("ball.y %f", ball.y);
    }
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(TextExample.getFont());
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}