#include "button.h"
#include <stdio.h>
#include <stdlib.h>

Button* createButton(int x, int y, int w, int h, SDL_Color originalColor, SDL_Color clickedColor, SDL_Color textColor, char* text, TTF_Font* font, SDL_Texture* texture){
    Button* button = (Button*)malloc(sizeof(Button));
    if (button == NULL) {
        fprintf(stderr, "Failed to allocate memory for button.\n");
        exit(EXIT_FAILURE);
    }
    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = w;
    button->rect.h = h;
    button->originalColor = originalColor;
    button->clickedColor = clickedColor;
    button->isClicked = false;
    button->clickedTime = 0;
    button->textColor = textColor;
    button->text = text;
    button->font = font;
    button->texture = texture;
    return button;
}

void destroyButton(Button* button) {
    free(button);
}

void handleClick(Button* button, int x, int y) {
    if (x >= button->rect.x && x <= button->rect.x + button->rect.w &&
        y >= button->rect.y && y <= button->rect.y + button->rect.h) {
        button->isClicked = true;
        button->clickedTime = SDL_GetTicks();
    }
}

void updateButton(Button* button) {
    if (button->isClicked && SDL_GetTicks() - button->clickedTime >= 1000) {
        button->isClicked = false;
    }
}

void renderButton(SDL_Renderer* renderer, Button* button) {
    if (button->isClicked) {
        SDL_SetRenderDrawColor(renderer, button->clickedColor.r, button->clickedColor.g,
                               button->clickedColor.b, button->clickedColor.a);
    } else {
        SDL_SetRenderDrawColor(renderer, button->originalColor.r, button->originalColor.g,
                               button->originalColor.b, button->originalColor.a);
    }
    SDL_RenderFillRect(renderer, &button->rect);
}

void renderText(SDL_Renderer* renderer, Button* button) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(button->font, button->text, button->textColor);
    button->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, button->texture, NULL, &button->rect);
}