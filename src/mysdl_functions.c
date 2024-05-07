#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "button.h"
#include "mysql_functions.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define FONT_SIZE 24

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;

void init_mysdl() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("PimenSlack",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    if (TTF_Init() != 0) {
        printf("TTF_Init error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    font = TTF_OpenFont("arial.ttf", FONT_SIZE);
    if (!font) {
        printf("TTF_OpenFont error: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}


void main_loop() {
    SDL_Event event;
    int quit = 0;

    SDL_Color originalColor = {217, 217, 217, 255};
    SDL_Color clickedColor = {255, 255, 255, 255};
    SDL_Color textColor = {0, 0, 0, 255};
    font = TTF_OpenFont("InknutAntiqua-Bold.ttf", 30);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "Click me!", textColor));


    Button *button = createButton(SCREEN_WIDTH / 2 - 100 / 2,
                                  SCREEN_HEIGHT / 2 - 50 / 2,
                                  100, 50,
                                  originalColor, clickedColor,
                                  textColor, "Click me!", font, texture
    );
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    handleClick(button, event.button.x, event.button.y);
                    break;
            }
        }

        updateButton(button);

        SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
        SDL_RenderClear(renderer);

        renderButton(renderer, button);
        renderText(renderer, button);

        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    destroyButton(button);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
}





