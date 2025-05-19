#include "mario.h"
#include "goomba.h"

int cameraX = 0;

void reinitialiser_carte(char map[H_MAP][L_MAP], char map_original[H_MAP][L_MAP]) {
	// Copier la carte originale dans la carte de jeu
	for (int y = 0; y < H_MAP; y++) {
		for (int x = 0; x < L_MAP; x++) {
			map[y][x] = map_original[y][x];
		}
	}
}
// Fonction pour initialiser l'affichage et dessiner la carte
void affichage(char map[H_MAP][L_MAP]) {

	printf("\033[H"); // Curseur en haut

	char ligne[L_SCREEN + 1];
	ligne[L_SCREEN] = '\0';

	for (int i = 0; i < H_SCREEN; i++) {
		for (int j = 0; j < L_SCREEN; j++) {
			ligne[j] = map[i][j + cameraX];
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
		// Gestion du saut (à faire)
	}

	affichage(map);
}

// Fonction pour vérifier si un bloc est solide
int est_solide(int x, int y, char map[H_MAP][L_MAP]) {
	// Vérifier si la position est en dehors de la carte
	if (x < 0 || x >= L_MAP || y < 0 || y >= H_MAP) {
		return 1; // Considérer les bords comme solides
	}
	// Vérifier le type de bloc à cette position
	char bloc = map[y][x];
	// Définir quels caractères représentent des blocs solides
	return (bloc == '#' || bloc == '|' || bloc == '_' || bloc == '0' || bloc == '+');
}

// Fonction pour appliquer la gravité
void appliquer_gravite(int en_saut, int camera_x, int* position_y, char map[H_MAP][L_MAP]) {
	int map_x = camera_x + PERSO_X;
	// Si le joueur n'est pas en saut et qu'il n'y a pas de bloc solide en dessous
	if (!en_saut && !est_solide(map_x, *position_y + 1, map)) {
		(*position_y)++; // Faire tomber le joueur
	}
}

// Version originale de la fonction dessiner (pour compatibilité avec le code existant)
void dessiner(int camera_x, int position_y, char map[H_MAP][L_MAP], int en_saut, int phase_saut, int nbPiece, int score, Goomba* g, int nb_goombas) {
	// On appelle notre nouvelle fonction avec la position_x au centre (PERSO_X)
	dessiner_avec_position_x(camera_x, position_y, PERSO_X, map, en_saut, phase_saut, nbPiece, score, g, nb_goombas);
}

// Nouvelle fonction dessiner qui prend en compte la position_x du personnage
void dessiner_avec_position_x(int camera_x, int position_y, int position_x, char map[H_MAP][L_MAP], int en_saut, int phase_saut, int nbPiece, int score, Goomba* goombas, int nb_goombas) {
	// Au lieu d'effacer l'écran complètement, on place simplement le curseur en haut
	printf("\033[H"); // Place le curseur en haut de l'écran sans effacer

	// Afficher toujours le scoreboard
	scoreboard(nbPiece, score);

	char ligne[L_SCREEN + 1];
	ligne[L_SCREEN] = '\0';

	for (int i = 0; i < H_SCREEN; i++) {
		for (int j = 0; j < L_SCREEN; j++) {
			int map_x = j + camera_x;

			// Vérifier si c'est un Goomba
			int goomba_trouve = 0;
			for (int k = 0; k < nb_goombas; k++) {
				if (goombas[k].x >= camera_x && goombas[k].x < camera_x + L_SCREEN &&
					goombas[k].x - camera_x == j && i == goombas[k].y) {
					ligne[j] = '@';
					goomba_trouve = 1;
					break;
				}
			}
			if (goomba_trouve) continue;

			// Vérifier si c'est le personnage
			if (j == position_x && (i == position_y || i == position_y - 1 || i == position_y - 2)) {
				if (i == position_y) ligne[j] = '^';
				else if (i == position_y - 1) ligne[j] = 'T';
				else if (i == position_y - 2) ligne[j] = 'O';
			}
			// Sinon, afficher la carte
			else if (map_x >= 0 && map_x < L_MAP && i < H_MAP) {
				ligne[j] = map[i][map_x];
			}
			else {
				ligne[j] = ' ';
			}
		}
		printf("%s\n", ligne);
	}

	// Flush stdout pour s'assurer que tout est affiché immédiatement
	fflush(stdout);
}