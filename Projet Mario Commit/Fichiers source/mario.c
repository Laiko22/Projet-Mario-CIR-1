#include "mario.h"
#include "goomba.h"

int cameraX = 0;

void reinitialiser_carte(char map[H_MAP][L_MAP], char map_original[H_MAP][L_MAP]) {
	for (int y = 0; y < H_MAP; y++) {
		for (int x = 0; x < L_MAP; x++) {
			map[y][x] = map_original[y][x];
		}
	}
}

void affichage(char map[H_MAP][L_MAP]) {
	printf("\033[H");

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
	if (touche == 75) {
		if (cameraX > 0)
			cameraX--;
	}
	else if (touche == 77) {
		if (cameraX < L_MAP - L_SCREEN)
			cameraX++;
	}
	else if (touche == 32) {
	}

	affichage(map);
}

int est_solide(int x, int y, char map[H_MAP][L_MAP]) {
	if (x < 0 || x >= L_MAP || y < 0 || y >= H_MAP) {
		return 1;
	}
	char bloc = map[y][x];
	return (bloc == '#' || bloc == '|' || bloc == '_' || bloc == '0' || bloc == '+');
}

void appliquer_gravite(int en_saut, int camera_x, int* position_y, char map[H_MAP][L_MAP]) {
	int map_x = camera_x + PERSO_X;
	if (!en_saut && !est_solide(map_x, *position_y + 1, map)) {
		(*position_y)++;
	}
}

void dessiner(int camera_x, int position_y, char map[H_MAP][L_MAP], int en_saut, int phase_saut, int nbPiece, int score, Goomba* g, int nb_goombas) {
	dessiner_avec_position_x(camera_x, position_y, PERSO_X, map, en_saut, phase_saut, nbPiece, score, g, nb_goombas);
}

void dessiner_avec_position_x(int camera_x, int position_y, int position_x, char map[H_MAP][L_MAP], int en_saut, int phase_saut, int nbPiece, int score, Goomba* goombas, int nb_goombas) {
	printf("\033[H");

	scoreboard(nbPiece, score);

	char ligne[L_SCREEN + 1];
	ligne[L_SCREEN] = '\0';

	for (int i = 0; i < H_SCREEN; i++) {
		for (int j = 0; j < L_SCREEN; j++) {
			int map_x = j + camera_x;

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

			if (j == position_x && (i == position_y || i == position_y - 1 || i == position_y - 2)) {
				if (i == position_y) ligne[j] = '^';
				else if (i == position_y - 1) ligne[j] = 'T';
				else if (i == position_y - 2) ligne[j] = 'O';
			}
			else if (map_x >= 0 && map_x < L_MAP && i < H_MAP) {
				ligne[j] = map[i][map_x];
			}
			else {
				ligne[j] = ' ';
			}
		}
		printf("%s\n", ligne);
	}

	fflush(stdout);
}
