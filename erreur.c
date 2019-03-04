#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "erreur.h"

void detruireSiErreur(SDL_Window* pWindow, SDL_Renderer* pRenderer)
{
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    exit(EXIT_FAILURE);
}
