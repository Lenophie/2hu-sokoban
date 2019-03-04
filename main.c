#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <fmod.h>
#include "constantes.h"
#include "erreur.h"
#include "jeu.h"

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Echec de l'initialisation de la SDL (%s)\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    FMOD_SYSTEM* pSystemFMOD;
    FMOD_System_Create(&pSystemFMOD);
    FMOD_System_Init(pSystemFMOD, 20, FMOD_INIT_NORMAL, NULL);

    SDL_Window* pWindow = NULL;
    SDL_Renderer* pRenderer = NULL;
    SDL_Surface* pSurfaceMenu = NULL;
    SDL_Texture* pTextureMenu = NULL;
    SDL_Rect position;

    SDL_Event event;
    int afficherFenetre = 1;

    pWindow = SDL_CreateWindow
    (
        "2hu sokoban",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH_WINDOW,
        HEIGHT_WINDOW,
        SDL_WINDOW_SHOWN
    );

    if (!pWindow)
    {
        fprintf(stderr, "Echec de l'initialisation de la fenetre (%s)\n", SDL_GetError());
        detruireSiErreur(pWindow, pRenderer);
    }

    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

    if (!pRenderer)
    {
        fprintf(stderr, "Echec de la creation du renderer (%s)", SDL_GetError());
        detruireSiErreur(pWindow, pRenderer);
    }

    pSurfaceMenu = SDL_LoadBMP("sprites/menu.bmp");

    if (!pSurfaceMenu)
    {
        fprintf(stderr, "Echec de l'initialisation de la surface menu (%s)", SDL_GetError());
        detruireSiErreur(pWindow, pRenderer);
    }

    pTextureMenu = SDL_CreateTextureFromSurface(pRenderer, pSurfaceMenu);
    SDL_FreeSurface(pSurfaceMenu);

    position.x = 0;
    position.y = 0;
    SDL_QueryTexture(pTextureMenu, NULL, NULL, &position.w, &position.h);
    SDL_RenderCopy(pRenderer, pTextureMenu, NULL, &position);
    SDL_RenderPresent(pRenderer);

    while (afficherFenetre)
    {
        if (SDL_WaitEvent(&event))
        {
            switch (event.type)
            {
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    afficherFenetre = 0;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    afficherFenetre = 0;
                    break;
                case SDLK_KP_ENTER:
                    demarrerPartie(pWindow, pRenderer, pSystemFMOD);
                    afficherFenetre = 0;
                    break;
                }
            }
        }
    }

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    FMOD_System_Close(pSystemFMOD);
    FMOD_System_Release(pSystemFMOD);

    return EXIT_SUCCESS;
}
