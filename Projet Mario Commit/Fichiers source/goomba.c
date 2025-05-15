#include "goomba.h"
#include "mario.h"

void initialiser_goomba(Goomba* g, int x, int y) {
	g->x = x;
	g->y = y;
	g->direction = -1; // commence par aller vers la gauche
	g->borne_gauche = x - 5;
	g->borne_droite = x + 5;
}

void deplacer_goomba(Goomba* g, char map[H_MAP][L_MAP]) {
	if (!est_solide(g->x, g->y + 1, map)) {
		g->y++; // gravité
	}
	else {
		int next_x = g->x + g->direction;

		if (next_x < g->borne_gauche || next_x > g->borne_droite) {
			g->direction *= -1;
		}
		else if (!est_solide(next_x, g->y, map) && est_solide(next_x, g->y + 1, map)) {
			g->x = next_x;
		}
		else {
			g->direction *= -1;
		}
	}
}

void dessiner_goomba(Goomba g, int camera_x, HANDLE hConsole, int i, int j) {
	WORD brownColor = FOREGROUND_RED | FOREGROUND_GREEN;
	WORD defaultColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	if (g.x >= camera_x && g.x < camera_x + L_SCREEN && g.x - camera_x == j && i == g.y) {
		SetConsoleTextAttribute(hConsole, brownColor);
		printf("@");
		SetConsoleTextAttribute(hConsole, defaultColor);
	}
}