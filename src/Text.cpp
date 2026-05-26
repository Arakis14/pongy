#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class Text {
 public:
  Text(const std::string& content) : Font{
    TTF_OpenFont("Roboto-Medium.ttf", 50.0f)
  } {}

  ~Text() {
    TTF_CloseFont(Font);
  }
  Text(const Text&) = delete;
  Text& operator=(const Text&) = delete;

private:
  TTF_Font* Font{nullptr};
};