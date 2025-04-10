#include "mario.h"

void affichage(char map[H_MAP][L_MAP]) {

	for (int i = 0; i < H_SCREEN; i++) {
		for (int j = 0; j < L_SCREEN; j++ ) {
			if (map[i][j] == '.')
			{
				printf("\033[30m%c\033[0m", map[i][j]);
			}
			else
			{
				printf("%c", map[i][j]);
			}
		}
		printf("\n");
		
	}

	if (_kbhit()) {
		char touche = _getch();

		if (touche == 75) { //FLECHE GAUCHE
			
		}
		else if (touche == 77) { //FLECH DROITE
			
		}
		else if (touche == 32) { // ESPACE

		}
	}
}
