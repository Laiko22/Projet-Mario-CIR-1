#include "mario.h"
#include "goomba.h"

int cameraX = 0;

// Fonction pour initialiser l'affichage et dessiner la carte
void affichage(char map[H_MAP][L_MAP]) {
	// Positionnement du curseur en haut � gauche
	printf("\033[H");

	// Effacer compl�tement l'�cran pour �viter les duplications
	printf("\033[2J");

	char ligne[L_SCREEN + 1];
	ligne[L_SCREEN] = '\0';

	for (int i = 0; i < H_SCREEN; i++) {
		for (int j = 0; j < L_SCREEN; j++) {
			if (j + cameraX < L_MAP) {
				ligne[j] = map[i][j + cameraX];
			}
			else {
				ligne[j] = ' '; // Pour �viter les d�passements de tableau
			}
		}
		printf("%s\n", ligne);
	}
}


void commande(int touche, char map[H_MAP][L_MAP]) {
	if (touche == 75) { // GAUCHE
		if (cameraX > 0)
			cameraX--;
	}
	else if (touche == 77) { // DROITE
		if (cameraX < L_MAP - L_SCREEN)
			cameraX++;
	}
	else if (touche == 32) {
		// Gestion du saut (� faire)
	}

	affichage(map);
}

// Fonction pour v�rifier si un bloc est solide
int est_solide(int x, int y, char map[H_MAP][L_MAP]) {
	// V�rifier si la position est en dehors de la carte
	if (x < 0 || x >= L_MAP || y < 0 || y >= H_MAP) {
		return 1; // Consid�rer les bords comme solides
	}
	// V�rifier le type de bloc � cette position
	char bloc = map[y][x];
	// D�finir quels caract�res repr�sentent des blocs solides
	return (bloc == '#' || bloc == '|' || bloc == '_' || bloc == '0' || bloc == '+');
}

// Fonction pour appliquer la gravit�
void appliquer_gravite(int en_saut, int camera_x, int* position_y, char map[H_MAP][L_MAP]) {
	int map_x = camera_x + PERSO_X;
	// Si le joueur n'est pas en saut et qu'il n'y a pas de bloc solide en dessous
	if (!en_saut && !est_solide(map_x, *position_y + 1, map)) {
		(*position_y)++; // Faire tomber le joueur
	}
}

// Version originale de la fonction dessiner (pour compatibilit� avec le code existant)
void dessiner(int camera_x, int position_y, char map[H_MAP][L_MAP], int en_saut, int phase_saut, int nbPiece, int score, Goomba g) {
	// On appelle notre nouvelle fonction avec la position_x au centre (PERSO_X)
	dessiner_avec_position_x(camera_x, position_y, PERSO_X, map, en_saut, phase_saut, nbPiece, score, g);
}

// Nouvelle fonction dessiner qui prend en compte la position_x du personnage
void dessiner_avec_position_x(int camera_x, int position_y, int position_x, char map[H_MAP][L_MAP], int en_saut, int phase_saut, int nbPiece, int score, Goomba g) {
	// Effacer l'�cran compl�tement � chaque fois pour �viter les duplications
	printf("\033[H"); // Positionnement du curseur en haut
	printf("\033[2J"); // Code d'effacement complet de l'�cran

	// Mettre � jour le scoreboard
	scoreboard(nbPiece, score);

	char ligne[L_SCREEN + 1];
	ligne[L_SCREEN] = '\0';

	for (int i = 0; i < H_SCREEN; i++) {
		for (int j = 0; j < L_SCREEN; j++) {
			// Position dans la carte
			int map_x = j + camera_x;

			// Initialiser avec un espace par d�faut
			ligne[j] = ' ';

			// V�rifier si c'est le Goomba
			if (g.x >= camera_x && g.x < camera_x + L_SCREEN && g.x - camera_x == j && i == g.y) {
				ligne[j] = '@'; // Caract�re pour le Goomba
			}
			// V�rifier si c'est le personnage
			else if (j == position_x && (i == position_y || i == position_y - 1 || i == position_y - 2)) {
				if (i == position_y) ligne[j] = '^';      // Pieds
				else if (i == position_y - 1) ligne[j] = 'T'; // Corps
				else if (i == position_y - 2) ligne[j] = 'O'; // T�te
			}
			// Sinon, afficher la carte
			else if (map_x >= 0 && map_x < L_MAP && i < H_MAP) {
				ligne[j] = map[i][map_x];
			}
		}
		printf("%s\n", ligne);
	}
}
