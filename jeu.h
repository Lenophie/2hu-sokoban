#ifndef DEF_JEU
#define DEF_JEU

void demarrerPartie(SDL_Window* pWindow, SDL_Renderer* pRenderer, FMOD_SYSTEM* pSystemFMOD);
void initialiserCarteNiveau(FILE* fichierNiveau, char carteNiveau[][NOMBRE_CASES_AXE_HORIZONTAL]);
void actualiserCarteNiveau(char carteNiveau[][NOMBRE_CASES_AXE_HORIZONTAL], int direction, int positionReimu[2], FMOD_SYSTEM* pSystemFMOD, FMOD_SOUND* pSonCaisse);
void actualiserAffichageNiveau(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Texture* pTextures[], SDL_Rect position, int direction, char carteNiveau[][NOMBRE_CASES_AXE_HORIZONTAL], int positionReimu[2]);
void verifierVictoire(char carteNiveau[][NOMBRE_CASES_AXE_HORIZONTAL], int* victoire);
void afficherVictoire(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Texture* pTextureVictoire, SDL_Rect position);
void afficherCarte(char carteNiveau[][NOMBRE_CASES_AXE_HORIZONTAL]);

#endif // DEF_JEU
