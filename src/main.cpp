#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <cstdlib>
#include <string>

//userinput speed?
const uint8_t speedMulti = 2;
double deltaTime = 0;
const uint16_t resolutionWidth = 1024;
const uint16_t resolutionHeight = 768;
const char* fontPath = "../assets/Roboto-Medium.ttf";
uint8_t scorePlayer1 = 0;
uint8_t scorePlayer2 = 0;
SDL_Surface* textSurface1;
SDL_Surface* textSurface2;
SDL_Texture *texture1;
SDL_Texture *texture2;
bool scoreChanged = false;

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

void increaseHeight(SDL_FRect& paddle)
{
    paddle.y -= 2 * speedMulti * deltaTime;
    //DEBUG
    //SDL_Log("New paddle y: %f", paddle.y);
}

void decreaseHeight(SDL_FRect& paddle)
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

void resetVelocity(Vec2& velocity)
{
    velocity.x = 0.1;
    velocity.y = 0.1;
}

void increaseVelocity(Vec2& velocity)
{
    velocity.x += 0.05;
    velocity.y += 0.05;
}

void decreaseVelocity(Vec2& velocity)
{
    velocity.x -= 0.05;
    velocity.y -= 0.05;
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
        scoreChanged = true;
        resetVelocity(velocity);
        resetBall(ball);
    }
    //Right wall
    if (ball.x <= 0) 
    {
        scorePlayer1++;
        scoreChanged = true;
        resetVelocity(velocity);
        resetBall(ball);
    }

    if (SDL_HasRectIntersectionFloat(&ball, &paddleLeft))
    {
        decreaseVelocity(velocity);
        velocity.x = std::abs(velocity.x);
        ball.x = paddleLeft.x + paddleLeft.w;
    }

    if (SDL_HasRectIntersectionFloat(&ball, &paddleRight))
    {
        increaseVelocity(velocity);
        velocity.x = -std::abs(velocity.x);
        ball.x = paddleRight.x - ball.w;
    }

}

bool checkIfGameOver()
{
    return (scorePlayer1 >=3 || scorePlayer2 >= 3);
}

SDL_Surface* createSurface(TTF_Font* font, uint8_t& score, SDL_Color colour)
{
    return TTF_RenderText_Blended(font, std::to_string(score).c_str(), 0, colour);
}

SDL_Texture* createTexture(SDL_Renderer* renderer, SDL_Surface* surface)
{
    return SDL_CreateTextureFromSurface(renderer, surface);
}

int main()
{
    //initialize variables
    bool quit_the_app = false;
    SDL_Color whiteColour = {255, 255, 255, 255};
    SDL_Window *window;
    SDL_FRect paddleLeft = {50, 50, 50, 100};
    SDL_FRect paddleRight = {924, 50, 50, 100};
    SDL_FRect ball = {512, 360, 15, 15};
    //Note: make rand starting velocity
    //float randomSeed = SDL_randf();
    Vec2 ballVelocity = {0.1, 0.1};
    SDL_Renderer *renderer;
    SDL_Event event;
    TTF_Font* font;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    TTF_Init();
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    //window
    if (!SDL_CreateWindowAndRenderer("Hello SDL", resolutionWidth, resolutionHeight, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
    //font
    font = TTF_OpenFont(fontPath, 48.0f);
    //text surface
    textSurface1 = createSurface(font, scorePlayer1, whiteColour);
    textSurface2 = createSurface(font, scorePlayer2, whiteColour);
    //textSurface1 = TTF_RenderText_Blended(font, std::to_string(scorePlayer1).c_str(), 0, whiteColour);
    //textSurface2 = TTF_RenderText_Blended(font, std::to_string(scorePlayer2).c_str(), 0, whiteColour);

    //texture
    texture1 = createTexture(renderer, textSurface1);
    texture2 = createTexture(renderer, textSurface2);

    SDL_FRect dstRect1{
        150.0f,
        100.0f,
        static_cast<float>(textSurface1->w),
        static_cast<float>(textSurface1->h)
    };

    SDL_FRect dstRect2{
        850.0f,
        100.0f,
        static_cast<float>(textSurface2->w),
        static_cast<float>(textSurface2->h)
    };

    SDL_DestroySurface(textSurface1);
    SDL_DestroySurface(textSurface2);

    while (!quit_the_app) {
        if(scoreChanged) {
            textSurface1 = createSurface(font, scorePlayer1, whiteColour);
            textSurface2 = createSurface(font, scorePlayer2, whiteColour);
            texture1 = createTexture(renderer, textSurface1);
            texture2 = createTexture(renderer, textSurface2);
            scoreChanged = false;
        }
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
        SDL_SetRenderTarget(renderer, texture1);
        SDL_SetRenderTarget(renderer, texture2);
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
        //SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderTexture(renderer, texture1, nullptr, &dstRect1);
        SDL_RenderTexture(renderer, texture2, nullptr, &dstRect2);
        SDL_RenderPresent(renderer);
        //loop
        changeBallPosition(ball, ballVelocity, paddleLeft, paddleRight);
        if (checkIfGameOver()) {
            break;
        }
        //DEBUG
        //SDL_Log("paddleRight.x %f", paddleRight.x);
        //SDL_Log("paddleRight.y %f", paddleRight.y);
        //SDL_Log("ball.x %f", ball.x);
        //SDL_Log("ball.y %f", ball.y);
    }
    TTF_CloseFont(font);
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}