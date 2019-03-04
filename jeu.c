#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <fmod.h>
#include "constantes.h"
#include "erreur.h"
#include "jeu.h"

void demarrerPartie(SDL_Window* pWindow, SDL_Renderer* pRenderer, FMOD_SYSTEM* pSystemFMOD)
{
    SDL_Surface* pSurfaceBackground = NULL;
    SDL_Texture* pTextureBackground = NULL;

    SDL_Surface* pSurfaceReimuFront = NULL;
    SDL_Surface* pSurfaceReimuBack = NULL;
    SDL_Surface* pSurfaceReimuLeft = NULL;
    SDL_Surface* pSurfaceReimuRight = NULL;
    SDL_Surface* pSurfaceMur = NULL;
    SDL_Surface* pSurfaceCaisse = NULL;
    SDL_Surface* pSurfaceCaisseOK = NULL;
    SDL_Surface* pSurfaceInterrupteur = NULL;

    SDL_Surface* pSurfaceVictoire = NULL;

    SDL_Texture* pTextures[9];

    SDL_Texture* pTextureReimuFront = NULL;
    SDL_Texture* pTextureReimuBack = NULL;
    SDL_Texture* pTextureReimuLeft = NULL;
    SDL_Texture* pTextureReimuRight = NULL;
    SDL_Texture* pTextureMur = NULL;
    SDL_Texture* pTextureCaisse = NULL;
    SDL_Texture* pTextureCaisseOK = NULL;
    SDL_Texture* pTextureInterrupteur = NULL;

    SDL_Texture* pTextureVictoire = NULL;

    int direction = TEX_REIMU_FRONT;

    SDL_Rect position;

    int victoire = 0, fini = 0, choixContinuer = 0;
    int niveauActuel = 1;
    SDL_Event event;

    FILE* fichierNiveau = NULL;
    char cheminNiveauActuel[18];
    char carteNiveau[NOMBRE_CASES_AXE_VERTICAL][NOMBRE_CASES_AXE_HORIZONTAL];
    int positionReimu[2];

    pSurfaceBackground = SDL_CreateRGBSurface(0, WIDTH_WINDOW, HEIGHT_WINDOW, 32, 0, 0, 0, 0);
    SDL_FillRect(pSurfaceBackground, NULL, SDL_MapRGB(pSurfaceBackground->format, 255, 255, 255));
    pTextureBackground = SDL_CreateTextureFromSurface(pRenderer, pSurfaceBackground);
    SDL_FreeSurface(pSurfaceBackground);
    position.x = 0;
    position.y = 0;
    SDL_QueryTexture(pTextureBackground, NULL, NULL, &position.w, &position.h);
    SDL_RenderCopy(pRenderer, pTextureBackground, NULL, &position);

    pSurfaceReimuFront = SDL_LoadBMP("sprites/reimufront.bmp");

    if (!pSurfaceReimuFront)
    {
        fprintf(stderr, "Impossible d'initialiser la surface reimufront (%s)", SDL_GetError());
        detruireSiErreur(pWindow, pRenderer);
    }

    SDL_SetColorKey(pSurfaceReimuFront, 1, SDL_MapRGB(pSurfaceReimuFront->format, 0, 255, 0));
    pTextureReimuFront = SDL_CreateTextureFromSurface(pRenderer, pSurfaceReimuFront);
    SDL_FreeSurface(pSurfaceReimuFront);

    if (!pTextureReimuFront)
    {
        fprintf(stderr, "Impossible d'initialiser la texture reimufront (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pSurfaceReimuBack = SDL_LoadBMP("sprites/reimuback.bmp");

    if (!pSurfaceReimuBack)
    {
        fprintf(stderr, "Impossible d'initialiser la surface reimuback (%s)", SDL_GetError());
        detruireSiErreur(pWindow, pRenderer);
    }

    SDL_SetColorKey(pSurfaceReimuBack, 1, SDL_MapRGB(pSurfaceReimuBack->format, 0, 255, 0));
    pTextureReimuBack = SDL_CreateTextureFromSurface(pRenderer, pSurfaceReimuBack);
    SDL_FreeSurface(pSurfaceReimuBack);

    if (!pTextureReimuBack)
    {
        fprintf(stderr, "Impossible d'initialiser la texture reimuback (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pSurfaceReimuLeft = SDL_LoadBMP("sprites/reimuleft.bmp");

    if (!pSurfaceReimuLeft)
    {
        fprintf(stderr, "Impossible d'initialiser la surface reimuleft (%s)", SDL_GetError());
        detruireSiErreur(pWindow, pRenderer);
    }

    SDL_SetColorKey(pSurfaceReimuLeft, 1, SDL_MapRGB(pSurfaceReimuLeft->format, 0, 255, 0));
    pTextureReimuLeft = SDL_CreateTextureFromSurface(pRenderer, pSurfaceReimuLeft);
    SDL_FreeSurface(pSurfaceReimuLeft);

    if (!pTextureReimuLeft)
    {
        fprintf(stderr, "Impossible d'initialiser la texture reimuleft (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pSurfaceReimuRight = SDL_LoadBMP("sprites/reimuright.bmp");

    if (!pSurfaceReimuRight)
    {
        fprintf(stderr, "Impossible d'initialiser la surface reimuright (%s)", SDL_GetError());
        detruireSiErreur(pWindow, pRenderer);
    }

    SDL_SetColorKey(pSurfaceReimuRight, 1, SDL_MapRGB(pSurfaceReimuRight->format, 0, 255, 0));
    pTextureReimuRight = SDL_CreateTextureFromSurface(pRenderer, pSurfaceReimuRight);
    SDL_FreeSurface(pSurfaceReimuRight);

    if (!pTextureReimuRight)
    {
        fprintf(stderr, "Impossible d'initialiser la texture reimuright (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pSurfaceMur = SDL_LoadBMP("sprites/mur.bmp");

    if (!pSurfaceMur)
    {
        fprintf(stderr, "Impossible de creer la surface mur (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pTextureMur = SDL_CreateTextureFromSurface(pRenderer, pSurfaceMur);
    SDL_FreeSurface(pSurfaceMur);

    if (!pTextureMur)
    {
        fprintf(stderr, "Impossible de creer la texture mur (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pSurfaceCaisse = SDL_LoadBMP("sprites/caisse.bmp");

    if (!pSurfaceCaisse)
    {
        fprintf(stderr, "Impossible de creer la surface caisse (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pTextureCaisse = SDL_CreateTextureFromSurface(pRenderer, pSurfaceCaisse);
    SDL_FreeSurface(pSurfaceCaisse);

    if (!pTextureCaisse)
    {
        fprintf(stderr, "Impossible de creer la texture caisse (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pSurfaceCaisseOK = SDL_LoadBMP("sprites/caisseok.bmp");

    if (!pSurfaceCaisseOK)
    {
        fprintf(stderr, "Impossible de creer la surface caisseok (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pTextureCaisseOK = SDL_CreateTextureFromSurface(pRenderer, pSurfaceCaisseOK);
    SDL_FreeSurface(pSurfaceCaisseOK);

    if (!pTextureCaisseOK)
    {
        fprintf(stderr, "Impossible de creer la texture caisseok (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pSurfaceInterrupteur = SDL_LoadBMP("sprites/interrupteur.bmp");

    if (!pSurfaceInterrupteur)
    {
        fprintf(stderr, "Impossible de creer la surface interrupteur (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_SetColorKey(pSurfaceInterrupteur, 1, SDL_MapRGB(pSurfaceInterrupteur->format, 0, 255, 0));
    pTextureInterrupteur = SDL_CreateTextureFromSurface(pRenderer, pSurfaceInterrupteur);
    SDL_FreeSurface(pSurfaceInterrupteur);

    if (!pTextureInterrupteur)
    {
        fprintf(stderr, "Impossible de creer la texture interrupteur (%s)", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pSurfaceVictoire = SDL_LoadBMP("sprites/gagne.bmp");

    if (!pSurfaceVictoire)
    {
        fprintf(stderr, "Impossible d'initialiser la surface victoire (%s)", SDL_GetError());
        detruireSiErreur(pWindow, pRenderer);
    }

    SDL_SetColorKey(pSurfaceVictoire, 1, SDL_MapRGB(pSurfaceVictoire->format, 0, 255, 0));
    pTextureVictoire = SDL_CreateTextureFromSurface(pRenderer, pSurfaceVictoire);
    SDL_FreeSurface(pSurfaceVictoire);

    pTextures[TEX_REIMU_FRONT] = pTextureReimuFront;
    pTextures[TEX_REIMU_BACK] = pTextureReimuBack;
    pTextures[TEX_REIMU_LEFT] = pTextureReimuLeft;
    pTextures[TEX_REIMU_RIGHT] = pTextureReimuRight;
    pTextures[TEX_MUR] = pTextureMur;
    pTextures[TEX_CAISSE] = pTextureCaisse;
    pTextures[TEX_CAISSE_OK] = pTextureCaisseOK;
    pTextures[TEX_INTERRUPTEUR] = pTextureInterrupteur;
    pTextures[TEX_BACKGROUND] = pTextureBackground;

    FMOD_SOUND* pSonCaisse = NULL;
    FMOD_System_CreateSound(pSystemFMOD, "music/SonCaisse.mp3", FMOD_CREATESAMPLE, 0, &pSonCaisse);

    if (!pSonCaisse)
    {
        fprintf(stderr, "Erreur de lecture de \"SonCaisse.mp3\"");
        detruireSiErreur(pWindow, pRenderer);
    }

    FMOD_SOUND* pSonVictoire = NULL;
    FMOD_CHANNEL* channelVictoire = NULL;
    FMOD_System_CreateSound(pSystemFMOD, "music/Victory.mp3", FMOD_CREATESAMPLE, 0, &pSonVictoire);

    if (!pSonVictoire)
    {
        fprintf(stderr, "Erreur de lecture de \"Victoire.mp3\"");
        detruireSiErreur(pWindow, pRenderer);
    }

    FMOD_System_GetChannel(pSystemFMOD, 1, &channelVictoire);

    FMOD_SOUND* pTheme = NULL;
    FMOD_CHANNEL* channelTheme = NULL;
    FMOD_System_CreateSound(pSystemFMOD, "music/Theme.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_CREATESAMPLE, 0, &pTheme);

    if (!pTheme)
    {
        fprintf(stderr, "Erreur de lecture de \"Theme.mp3\"");
        detruireSiErreur(pWindow, pRenderer);
    }

    FMOD_System_GetChannel(pSystemFMOD, 2, &channelTheme);

    FMOD_Sound_SetLoopCount(pTheme, -1);

    while (!fini)
    {
        sprintf(cheminNiveauActuel, "levels/level%d.txt", niveauActuel);
        fichierNiveau = fopen(cheminNiveauActuel,"r");

        if (!fichierNiveau)
        {
            fprintf(stderr, "Impossible d'ouvrir le fichier level");
            detruireSiErreur(pWindow, pRenderer);
        }

        initialiserCarteNiveau(fichierNiveau, carteNiveau);

        fclose(fichierNiveau);

        actualiserAffichageNiveau(pWindow, pRenderer, pTextures, position, direction, carteNiveau, positionReimu);
        FMOD_System_PlaySound(pSystemFMOD, pTheme, NULL, 0, &channelTheme);

        while (!victoire)
        {
            if (SDL_WaitEvent(&event))
            {
                switch (event.type)
                {
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        FMOD_Sound_Release(pSonCaisse);
                        FMOD_Sound_Release(pSonVictoire);
                        FMOD_Sound_Release(pTheme);
                        exit(EXIT_SUCCESS);
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        FMOD_Sound_Release(pSonCaisse);
                        FMOD_Sound_Release(pSonVictoire);
                        FMOD_Sound_Release(pTheme);
                        exit(EXIT_SUCCESS);
                        break;
                    case SDLK_UP:
                        actualiserCarteNiveau(carteNiveau, UP, positionReimu, pSystemFMOD, pSonCaisse);
                        actualiserAffichageNiveau(pWindow, pRenderer, pTextures, position, UP, carteNiveau, positionReimu);
                        break;
                    case SDLK_DOWN:
                        actualiserCarteNiveau(carteNiveau, DOWN, positionReimu, pSystemFMOD, pSonCaisse);
                        actualiserAffichageNiveau(pWindow, pRenderer, pTextures, position, DOWN, carteNiveau, positionReimu);
                        break;
                    case SDLK_LEFT:
                        actualiserCarteNiveau(carteNiveau, LEFT, positionReimu, pSystemFMOD, pSonCaisse);
                        actualiserAffichageNiveau(pWindow, pRenderer, pTextures, position, LEFT, carteNiveau, positionReimu);
                        break;
                    case SDLK_RIGHT:
                        actualiserCarteNiveau(carteNiveau, RIGHT, positionReimu, pSystemFMOD, pSonCaisse);
                        actualiserAffichageNiveau(pWindow, pRenderer, pTextures, position, RIGHT, carteNiveau, positionReimu);
                        break;
                    case SDLK_r:
                        fichierNiveau = fopen(cheminNiveauActuel,"r");

                        if (!fichierNiveau)
                        {
                            fprintf(stderr, "Impossible d'ouvrir le fichier level");
                            detruireSiErreur(pWindow, pRenderer);
                        }

                        initialiserCarteNiveau(fichierNiveau, carteNiveau);
                        fclose(fichierNiveau);

                        actualiserAffichageNiveau(pWindow, pRenderer, pTextures, position, DOWN, carteNiveau, positionReimu);
                        break;
                    }
                    verifierVictoire(carteNiveau, &victoire);
                }
            }
        }
        FMOD_Channel_Stop(channelTheme);
        afficherVictoire(pWindow, pRenderer, pTextureVictoire, position);
        FMOD_System_PlaySound(pSystemFMOD, pSonVictoire, NULL, 0, &channelVictoire);
        victoire = 0;

        while (!choixContinuer)
        {
            if (SDL_WaitEvent(&event))
            {
                switch (event.type)
                {
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        exit(EXIT_SUCCESS);
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        exit(EXIT_SUCCESS);
                        break;
                    case SDLK_KP_ENTER:
                        choixContinuer = 1;
                        FMOD_Channel_Stop(channelVictoire);
                    }
                }
            }
        }

        choixContinuer = 0;
        niveauActuel++;
        if (niveauActuel > NOMBRE_NIVEAUX)
        {
            fini = 1;
        }
    }

    FMOD_Sound_Release(pSonCaisse);
    FMOD_Sound_Release(pSonVictoire);
    FMOD_Sound_Release(pTheme);
}

void initialiserCarteNiveau(FILE* fichierNiveau, char carteNiveau[][NOMBRE_CASES_AXE_HORIZONTAL])
{
    char caractereLu;
    int k = 0, l = 0;

    do
    {
        caractereLu = fgetc(fichierNiveau);
        if (caractereLu != EOF)
        {
            if (caractereLu != '\n')
            {
                carteNiveau[k][l] = caractereLu;
                l++;
            }
            else
            {
                l = 0;
                k++;
            }
        }
    } while (caractereLu != EOF);
}

void actualiserCarteNiveau(char carteNiveau[][NOMBRE_CASES_AXE_HORIZONTAL], int direction, int positionReimu[2], FMOD_SYSTEM* pSystemFMOD, FMOD_SOUND* pSonCaisse)
{
    int backUpPositionReimu[2] = {positionReimu[0], positionReimu[1]};
    if (carteNiveau[positionReimu[1]][positionReimu[0]] == 'R')
    {
        carteNiveau[positionReimu[1]][positionReimu[0]] = ' ';
    }

    switch(direction)
    {
    case UP:
        positionReimu[1]--;
        break;
    case DOWN:
        positionReimu[1]++;
        break;
    case LEFT:
        positionReimu[0]--;
        break;
    case RIGHT:
        positionReimu[0]++;
        break;
    }

    if (positionReimu[0] * positionReimu[1] < 0 ||
        positionReimu[0] > NOMBRE_CASES_AXE_HORIZONTAL - 1 ||
        positionReimu[1] > NOMBRE_CASES_AXE_VERTICAL - 1 ||
        carteNiveau[positionReimu[1]][positionReimu[0]] == '*')
    {
        positionReimu[0] = backUpPositionReimu[0];
        positionReimu[1] = backUpPositionReimu[1];
        if (carteNiveau[positionReimu[1]][positionReimu[0]] == ' ')
        {
            carteNiveau[positionReimu[1]][positionReimu[0]] = 'R';
        }
        else
        {
            carteNiveau[positionReimu[1]][positionReimu[0]] = 'D';
        }
    }

    else if (carteNiveau[positionReimu[1]][positionReimu[0]] == ' ')
    {
        carteNiveau[positionReimu[1]][positionReimu[0]] = 'R';
    }

    else if (carteNiveau[positionReimu[1]][positionReimu[0]] == 'C' || carteNiveau[positionReimu[1]][positionReimu[0]] == 'O')
    {
        int nouvellePositionCaisse[2] = {0,0};

        switch(direction)
        {
        case LEFT:
            nouvellePositionCaisse[0] = positionReimu[0] - 1;
            nouvellePositionCaisse[1] = positionReimu[1];
            break;
        case RIGHT:
            nouvellePositionCaisse[0] = positionReimu[0] + 1;
            nouvellePositionCaisse[1] = positionReimu[1];
            break;
        case UP:
            nouvellePositionCaisse[0] = positionReimu[0];
            nouvellePositionCaisse[1] = positionReimu[1] - 1;
            break;
        case DOWN:
            nouvellePositionCaisse[0] = positionReimu[0];
            nouvellePositionCaisse[1] = positionReimu[1] + 1;
            break;
        }

        if (nouvellePositionCaisse[0] * nouvellePositionCaisse[1] < 0 ||
            nouvellePositionCaisse[0] > NOMBRE_CASES_AXE_HORIZONTAL - 1 ||
            nouvellePositionCaisse[1] > NOMBRE_CASES_AXE_VERTICAL - 1 ||
            carteNiveau[nouvellePositionCaisse[1]][nouvellePositionCaisse[0]] == '*' ||
            carteNiveau[nouvellePositionCaisse[1]][nouvellePositionCaisse[0]] == 'C' ||
            carteNiveau[nouvellePositionCaisse[1]][nouvellePositionCaisse[0]] == 'O')
        {
            positionReimu[0] = backUpPositionReimu[0];
            positionReimu[1] = backUpPositionReimu[1];
            if (carteNiveau[positionReimu[1]][positionReimu[0]] == ' ')
            {
                carteNiveau[positionReimu[1]][positionReimu[0]] = 'R';
            }
            else
            {
                carteNiveau[positionReimu[1]][positionReimu[0]] = 'D';
            }
        }

        else if (carteNiveau[nouvellePositionCaisse[1]][nouvellePositionCaisse[0]] == 'I')
        {
            carteNiveau[nouvellePositionCaisse[1]][nouvellePositionCaisse[0]] = 'O';

            if (carteNiveau[positionReimu[1]][positionReimu[0]] == 'O')
            {
                carteNiveau[positionReimu[1]][positionReimu[0]] = 'D';
            }

            else if (carteNiveau[positionReimu[1]][positionReimu[0]] == 'C')
            {
                carteNiveau[positionReimu[1]][positionReimu[0]] = 'R';
            }

            FMOD_System_PlaySound(pSystemFMOD, pSonCaisse, NULL, 0, NULL);
            FMOD_System_Update(pSystemFMOD);
        }

        else if (carteNiveau[nouvellePositionCaisse[1]][nouvellePositionCaisse[0]] == ' ')
        {
            carteNiveau[nouvellePositionCaisse[1]][nouvellePositionCaisse[0]] = 'C';

            if (carteNiveau[positionReimu[1]][positionReimu[0]] == 'O')
            {
                carteNiveau[positionReimu[1]][positionReimu[0]] = 'D';
            }

            else if (carteNiveau[positionReimu[1]][positionReimu[0]] == 'C')
            {
                carteNiveau[positionReimu[1]][positionReimu[0]] = 'R';
            }

            FMOD_System_PlaySound(pSystemFMOD, pSonCaisse, NULL, 0, NULL);
            FMOD_System_Update(pSystemFMOD);
        }
    }

    else if (carteNiveau[positionReimu[1]][positionReimu[0]] == 'I')
    {
        carteNiveau[positionReimu[1]][positionReimu[0]] = 'D';
    }

    if (carteNiveau[backUpPositionReimu[1]][backUpPositionReimu[0]] == 'D' && (backUpPositionReimu[1] != positionReimu[1] || backUpPositionReimu[0] != positionReimu[0]))
    {
        carteNiveau[backUpPositionReimu[1]][backUpPositionReimu[0]] = 'I';
    }
}

void actualiserAffichageNiveau(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Texture* pTextures[], SDL_Rect position, int direction, char carteNiveau[][NOMBRE_CASES_AXE_HORIZONTAL], int positionReimu[2])
{
    char caractereLu;
    int k = 0, l = 0;

    SDL_RenderClear(pRenderer);

    position.x = 0;
    position.y = 0;
    SDL_QueryTexture(pTextures[TEX_BACKGROUND], NULL, NULL, &position.w, &position.h);
    SDL_RenderCopy(pRenderer, pTextures[TEX_BACKGROUND], NULL, &position);

    for (k = 0; k < NOMBRE_CASES_AXE_VERTICAL; k++)
    {
        for (l = 0; l < NOMBRE_CASES_AXE_HORIZONTAL; l++)
        {
            caractereLu = carteNiveau[k][l];

            position.x = l*80;
            position.y = k*80;

            switch (caractereLu)
            {
            case 'R':
                SDL_QueryTexture(pTextures[direction], NULL, NULL, &position.w, &position.h);
                SDL_RenderCopy(pRenderer, pTextures[direction], NULL, &position);
                positionReimu[0] = l;
                positionReimu[1] = k;
                break;
            case '*':
                SDL_QueryTexture(pTextures[TEX_MUR], NULL, NULL, &position.w, &position.h);
                SDL_RenderCopy(pRenderer, pTextures[TEX_MUR], NULL, &position);
                break;
            case 'C':
                SDL_QueryTexture(pTextures[TEX_CAISSE], NULL, NULL, &position.w, &position.h);
                SDL_RenderCopy(pRenderer, pTextures[TEX_CAISSE], NULL, &position);
                break;
            case 'I':
                SDL_QueryTexture(pTextures[TEX_INTERRUPTEUR], NULL, NULL, &position.w, &position.h);
                SDL_RenderCopy(pRenderer, pTextures[TEX_INTERRUPTEUR], NULL, &position);
                break;
            case 'O':
                SDL_QueryTexture(pTextures[TEX_CAISSE_OK], NULL, NULL, &position.w, &position.h);
                SDL_RenderCopy(pRenderer, pTextures[TEX_CAISSE_OK], NULL, &position);
                break;
            case 'D':
                SDL_QueryTexture(pTextures[TEX_INTERRUPTEUR], NULL, NULL, &position.w, &position.h);
                SDL_RenderCopy(pRenderer, pTextures[TEX_INTERRUPTEUR], NULL, &position);

                SDL_QueryTexture(pTextures[direction], NULL, NULL, &position.w, &position.h);
                SDL_RenderCopy(pRenderer, pTextures[direction], NULL, &position);
                positionReimu[0] = l;
                positionReimu[1] = k;
                break;
            }
        }
    }

    SDL_RenderPresent(pRenderer);
}

void verifierVictoire(char carteNiveau[][NOMBRE_CASES_AXE_HORIZONTAL], int* victoire)
{
    int k = 0, l = 0;
    *victoire = 1;

    for (k = 0; k < NOMBRE_CASES_AXE_VERTICAL; k++)
    {
        for (l = 0; l < NOMBRE_CASES_AXE_VERTICAL; l++)
        {
            if (carteNiveau[k][l] == 'I' || carteNiveau[k][l] == 'C')
            {
                *victoire = 0;
                break;
            }
        }

        if (*victoire == 0)
        {
            break;
        }
    }
}

void afficherVictoire(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Texture* pTextureVictoire, SDL_Rect position)
{
    SDL_QueryTexture(pTextureVictoire, NULL, NULL, &position.w, &position.h);
    position.x = WIDTH_WINDOW/2 - position.w/2;
    position.y = HEIGHT_WINDOW/2 - position.h/2;
    SDL_RenderCopy(pRenderer, pTextureVictoire, NULL, &position);
    SDL_RenderPresent(pRenderer);
}

void afficherCarte(char carteNiveau[][NOMBRE_CASES_AXE_HORIZONTAL])
{
    int k = 0, l = 0;

    for (k = 0; k < NOMBRE_CASES_AXE_VERTICAL; k++)
    {
        for (l = 0; l < NOMBRE_CASES_AXE_VERTICAL; l++)
        {
            printf("%c", carteNiveau[k][l]);
            if (l == NOMBRE_CASES_AXE_VERTICAL - 1)
            {
                printf("\n");
            }
        }
    }
    printf("\n\n");
}
