#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    SDL_Rect rect;
    SDL_Color originalColor;
    SDL_Color clickedColor;
    SDL_Color textColor;
    char* text;
    TTF_Font* font;
    SDL_Texture* texture;
    bool isClicked;
    uint32_t clickedTime;
} Button;

Button* createButton(int x, int y, int w, int h, SDL_Color originalColor, SDL_Color clickedColor, SDL_Color textColor, char* text, TTF_Font* font, SDL_Texture* texture);
void destroyButton(Button* button);
void handleClick(Button* button, int x, int y);
void updateButton(Button* button);
void renderButton(SDL_Renderer* renderer, Button* button);
void renderText(SDL_Renderer* renderer, Button* button);

#endif /* BUTTON_H */
