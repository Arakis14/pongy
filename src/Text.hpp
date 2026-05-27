
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

const char* fontPath = "../assets/Roboto-Medium.ttf";

class Text {
 public:
  Text(const std::string& content) :
    content(content),
    Font(TTF_OpenFont(fontPath, 50.0f))
  {}

  ~Text() {
    TTF_CloseFont(Font);
  }
  Text(const Text&) = delete;
  Text& operator=(const Text&) = delete;

  TTF_Font* getFont() { return Font; }
  std::string getText() { return content; }
private:
  std::string content;
  TTF_Font* Font{nullptr};
};