#include "mario.h"
#include "menu.h"
#include "jeu.h"
#include "menuEchap.h"
#include "goomba.h"
#include "tableauScores.h"
#include "vie_mort.h"
#include "map_original.h"
#include <windows.h> // Pour GetAsyncKeyState
#include "musiqueM.h"

void init_jeu(JeuState* etat, char map[H_MAP][L_MAP]) {
	etat->position_y = 14;
	etat->camera_x = 0;
	etat->position_x = 0;  // Position initiale du personnage � l'�cran (centre)
	etat->en_saut = 0;
	etat->phase_saut = 0;
	etat->quitter = 0;
	etat->direction = 0;
	etat->nbPiece = 0;  // Initialiser le compteur de pi�ces � 0
	init_vie_state(etat);
}


void deplacement_gauche(JeuState* etat, char map[H_MAP][L_MAP]) {
	// Si nous sommes au bord gauche de la carte et que le personnage n'est pas au bord gauche de l'�cran
	if (etat->camera_x == 0) {
		// Mode d�placement personnage (bord gauche)
		if (etat->position_x > 0) {
			int nouvelle_pos_x = etat->position_x - VITESSE_DEPLACEMENT;
			// V�rifier si le personnage ne va pas traverser un mur
			if (!est_solide(nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);
			}
		}
	}
	// Si nous sommes au bord droit de la carte
	else if (etat->camera_x == L_MAP - L_SCREEN) {
		// Mode d�placement personnage (bord droit)
		if (etat->position_x > 0) {
			int nouvelle_pos_x = etat->position_x - VITESSE_DEPLACEMENT;
			// V�rifier si le personnage ne va pas traverser un mur
			if (!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);
			}
		}
	}
	// Si nous sommes au milieu de la carte
	else {
		// Mode d�placement cam�ra
		int nouvelle_camera_x = etat->camera_x - VITESSE_DEPLACEMENT;
		if (nouvelle_camera_x >= 0) {
			int map_x = nouvelle_camera_x + etat->position_x;
			// V�rifier si le personnage ne va pas traverser un mur
			if (!est_solide(map_x, etat->position_y, map) &&
				!est_solide(map_x, etat->position_y - 1, map) &&
				!est_solide(map_x, etat->position_y - 2, map)) {
				etat->camera_x = nouvelle_camera_x;
				verifier_collision_pieces(etat, map);
			}
		}
	}
	etat->direction = -1;
}

void deplacement_droite(JeuState* etat, char map[H_MAP][L_MAP]) {
	// Si nous sommes au bord gauche de la carte
	if (etat->camera_x == 0) {
		// Mode d�placement personnage (bord gauche)
		if (etat->position_x < L_SCREEN - 1) {
			int nouvelle_pos_x = etat->position_x + VITESSE_DEPLACEMENT;
			// V�rifier si le personnage ne va pas traverser un mur
			if (!est_solide(nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);

				// Transition vers le mode cam�ra si on d�passe le centre
				if (etat->position_x > PERSO_X) {
					etat->camera_x += (etat->position_x - PERSO_X);
					etat->position_x = PERSO_X;
				}
			}
		}
	}
	// Si nous sommes au bord droit de la carte
	else if (etat->camera_x == L_MAP - L_SCREEN) {
		// Mode d�placement personnage (bord droit)
		if (etat->position_x < L_SCREEN - 1) {
			int nouvelle_pos_x = etat->position_x + VITESSE_DEPLACEMENT;
			// V�rifier si le personnage ne va pas traverser un mur
			if (!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 1, map) &&
				!est_solide(etat->camera_x + nouvelle_pos_x, etat->position_y - 2, map)) {
				etat->position_x = nouvelle_pos_x;
				verifier_collision_pieces(etat, map);
			}
		}
	}
	// Si nous sommes au milieu de la carte
	else {
		// Mode d�placement cam�ra
		int nouvelle_camera_x = etat->camera_x + VITESSE_DEPLACEMENT;
		if (nouvelle_camera_x <= L_MAP - L_SCREEN) {
			int map_x = nouvelle_camera_x + etat->position_x;
			// V�rifier si le personnage ne va pas traverser un mur
			if (!est_solide(map_x, etat->position_y, map) &&
				!est_solide(map_x, etat->position_y - 1, map) &&
				!est_solide(map_x, etat->position_y - 2, map)) {
				etat->camera_x = nouvelle_camera_x;

				// Si on atteint le bord droit de la carte, ajuster la position
				if (etat->camera_x > L_MAP - L_SCREEN) {
					etat->camera_x = L_MAP - L_SCREEN;
				}

				verifier_collision_pieces(etat, map);
			}
		}
	}
	etat->direction = 1;
}


void gestion_clavier(JeuState* etat, char map[H_MAP][L_MAP], int* nbPiece, int* score, Goomba* g, int nb_goombas) {

	etat->direction = 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		etat->direction = -1;
	}
	// VK_RIGHT = Fl�che droite
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		etat->direction = 1;
	}

	while (_kbhit()) {
		char touche = _getch();
		if (touche == 0 || touche == -32) {
			char fleche = _getch();

			// Fl�che haut (saut)
			if (fleche == 72 && !etat->en_saut) {
				int map_x = etat->camera_x + etat->position_x;
				if (est_solide(map_x, etat->position_y + 1, map)) {
					etat->en_saut = 1;
					etat->phase_saut = 0;
				}
			}
		}
		// Barre d'espace pour sauter
		else if (touche == 32 && !etat->en_saut) {
			int map_x = etat->camera_x + etat->position_x;
			if (est_solide(map_x, etat->position_y + 1, map)) {
				etat->en_saut = 1;
				etat->phase_saut = 0;
			}
		}
		// Touche �chap (menu pause)
		else if (touche == 27) {
			int resultat_menu = menuEchap(map);
			if (resultat_menu == 1) {
				etat->quitter = 1; // Quitter le jeu
				return;
			}

			printf("\033[H"); // D�placer le curseur au d�but sans effacer
			// Redessiner l'�cran
			dessiner_avec_position_x(etat->camera_x, etat->position_y, etat->position_x, map, etat->en_saut, etat->phase_saut, *nbPiece, *score, g, nb_goombas);
			afficher_vies(etat);
		}
	}

	// V�rifier aussi le saut avec GetAsyncKeyState
	if (!etat->en_saut && (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState(VK_SPACE) & 0x8000)) {
		int map_x = etat->camera_x + etat->position_x;
		if (est_solide(map_x, etat->position_y + 1, map)) {
			etat->en_saut = 1;
			etat->phase_saut = 0;
		}
	}
}

int verifier_fin_niveau(JeuState* etat, char map[H_MAP][L_MAP]) {
	int map_x = etat->camera_x + etat->position_x;

	// Chercher la position du caract�re '0'
	for (int x = 0; x < L_MAP; x++) {
		for (int y = 0; y < H_MAP; y++) {
			if (map[y][x] == '0') {
				// Si le joueur est juste devant le '0'
				if (map_x == x - 1 && etat->position_y == y) {
					return 1; // Niveau termin�
				}
				// Pas besoin de continuer la recherche
				return 0;
			}
		}
	}
	return 0; // Pas trouv� de '0' ou joueur pas � la position finale
}

void afficher_felicitations(int nbPiece, int score) {
	// Sauvegarder le score et les pi�ces pour que tableauScores puisse y acc�der
	sauvegarder_score_actuel(nbPiece, score);

	// Effacer l'�cran
	system("cls");

	// Dimensions du cadre
	int largeur = 50;
	int hauteur = 11;

	// Position pour centrer le message
	int pos_y = H_SCREEN / 2 - hauteur / 2;
	int pos_x = L_SCREEN / 2 - largeur / 2;

	// Messages � afficher
	char* messages[] = {
		"HERE WE GO !",
		"VOUS AVEZ FINI LE NIVEAU",
		"",
		"Appuyez sur une touche",
		"pour revenir au menu"
	};
	int nb_messages = sizeof(messages) / sizeof(messages[0]);

	// Dessiner la bordure sup�rieure
	printf("\033[%d;%dH", pos_y, pos_x);
	for (int i = 0; i < largeur; i++) {
		printf("*");
	}

	// Dessiner les lignes du milieu avec les messages
	int ligne_message = 0;
	for (int i = 1; i < hauteur - 1; i++) {
		printf("\033[%d;%dH", pos_y + i, pos_x);
		printf("*"); // Bordure gauche

		// Afficher un message si on est sur une ligne avec message
		if (i >= 2 && i < 2 + nb_messages && ligne_message < nb_messages) {
			int longueur_msg = strlen(messages[ligne_message]);
			int padding = (largeur - 2 - longueur_msg) / 2; // Centrer le message

			// Espaces avant le message
			for (int j = 0; j < padding; j++) {
				printf(" ");
			}

			// Le message
			printf("%s", messages[ligne_message]);
			ligne_message++;

			// Espaces apr�s le message (ajuster pour les impairs)
			for (int j = 0; j < largeur - 2 - padding - longueur_msg; j++) {
				printf(" ");
			}
		}
		else {
			// Ligne vide
			for (int j = 0; j < largeur - 2; j++) {
				printf(" ");
			}
		}

		printf("*"); // Bordure droite
	}

	// Dessiner la bordure inf�rieure
	printf("\033[%d;%dH", pos_y + hauteur - 1, pos_x);
	for (int i = 0; i < largeur; i++) {
		printf("*");
	}

	// Attendre une touche
	_getch();
}


void mise_a_jour_jeu(JeuState* etat, char map[H_MAP][L_MAP], int* nbPiece, int* score, Goomba* g, int nb_goombas) {
	verifier_mort(etat, map, *nbPiece, *score, g, nb_goombas);

	if (verifier_fin_niveau(etat, map)) {
		afficher_felicitations(*nbPiece, *score);
		etat->quitter = 1; // Quitter le jeu et retourner au menu principal
		return;
	}

	if (etat->direction == -1) {
		deplacement_gauche(etat, map);
	}
	else if (etat->direction == 1) {
		deplacement_droite(etat, map);
	}


	if (etat->en_saut) {
		int map_x = etat->camera_x + etat->position_x;
		if (etat->phase_saut < HAUTEUR_SAUT) {
			if (!est_solide(map_x, etat->position_y - 1, map)) {
				etat->position_y--;
				verifier_collision_pieces(etat, map);
			}
			else {
				etat->phase_saut = HAUTEUR_SAUT;
			}
			etat->phase_saut++;
		}
		else if (etat->phase_saut < 2 * HAUTEUR_SAUT) {
			if (!est_solide(map_x, etat->position_y + 1, map)) {
				etat->position_y++;
				verifier_collision_pieces(etat, map);
			}
			else {
				etat->en_saut = 0;
			}
			etat->phase_saut++;
		}
		else {
			etat->en_saut = 0;
		}
	}
	else {
		// Appliquer la gravit�
		int map_x = etat->camera_x + etat->position_x;
		if (!est_solide(map_x, etat->position_y + 1, map)) {
			etat->position_y++;
			verifier_collision_pieces(etat, map);
		}
	}

	// Mettre � jour nbPiece avec la valeur de l'�tat
	*nbPiece = etat->nbPiece;
}

void verifier_mort(JeuState* etat, char map[H_MAP][L_MAP], int nbPiece, int score, Goomba* g, int nb_goombas) {
	int statut_mort = est_mort(etat, map, g, nb_goombas);

	if (statut_mort > 0) {
		// Afficher le menu de mort et g�rer la r�ponse
		int resultat = menu_mort(etat);

		if (resultat == 1) {
			// Continuer en respawnant le joueur
			respawn_joueur(etat);
			// Redessiner l'�cran du jeu
			effacer_ecranE();
			dessiner(etat->camera_x, etat->position_y, map, etat->en_saut, etat->phase_saut, nbPiece, score, g, nb_goombas);
			afficher_vies(etat);
		}
		else {
			// Quitter et retourner au menu principal
			etat->quitter = 1;
		}
	}
}


void verifier_collision_pieces(JeuState* etat, char map[H_MAP][L_MAP]) {
	int map_x = etat->camera_x + etat->position_x;

	// V�rifier pour les trois parties du corps du personnage (t�te, torse, pieds)
	for (int partie = 0; partie < 3; partie++) {
		int pos_y = etat->position_y - partie;

		// Si la position est valide
		if (pos_y >= 0 && pos_y < H_MAP && map_x >= 0 && map_x < L_MAP) {
			// Si on trouve une pi�ce � cette position
			if (map[pos_y][map_x] == 'o') {
				// Collecte de la pi�ce
				map[pos_y][map_x] = '.';  // Remplacer la pi�ce par un espace vide
				etat->nbPiece++;  // Incr�menter le compteur de pi�ces
			}
		}
	}
}

void boucle_jeu(char map[H_MAP][L_MAP], int nbPiece, int score, Goomba* g, int nb_goombas) {
	reinitialiser_carte(map, map_original);
	JeuState etat;
	init_jeu(&etat, map);
	etat.nbPiece = nbPiece; // Initialiser avec la valeur fournie

	// Variables pour maintenir le score et le nombre de pi�ces actuels
	int nbPiece_actuel = etat.nbPiece;
	int score_actuel = score;

	// Effacer compl�tement l'�cran une fois au d�but
	system("cls");

	// Configuration pour r�duire le clignotement
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE; // Masquer le curseur
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	// Initialiser l'affichage
	dessiner_avec_position_x(etat.camera_x, etat.position_y, etat.position_x, map, etat.en_saut, etat.phase_saut, nbPiece_actuel, score_actuel, g, nb_goombas);

	int derniere_mise_a_jour = GetTickCount64();
	int taux_rafraichissement = 70; // 70ms entre chaque rafra�chissement

	continuer_musique = 1;
	//_beginthread(playMusic, 0, NULL);

	afficher_vies(&etat);

	while (!etat.quitter) {
		// Mettre � jour le score bas� sur la progression
		score_actuel = score + (etat.camera_x);

		// Sauvegarder le score actuel pour que d'autres modules puissent y acc�der
		sauvegarder_score_actuel(nbPiece_actuel, score_actuel);

		// Afficher le scoreboard
		scoreboard(nbPiece_actuel, score_actuel);

		// G�rer les entr�es clavier
		gestion_clavier(&etat, map, &nbPiece_actuel, &score_actuel, g, nb_goombas);

		// Mettre � jour la position du personnage et des ennemis
		mise_a_jour_jeu(&etat, map, &nbPiece_actuel, &score_actuel, g, nb_goombas);

		// Mettre � jour la position du Goomba
		for (int i = 0; i < nb_goombas; i++) {
			deplacer_goomba(&g[i], map);
		}

		// Redessiner l'�cran � un taux de rafra�chissement constant
		int temps_actuel = GetTickCount64();
		if (temps_actuel - derniere_mise_a_jour >= taux_rafraichissement) {
			dessiner_avec_position_x(etat.camera_x, etat.position_y, etat.position_x, map, etat.en_saut, etat.phase_saut, nbPiece_actuel, score_actuel, g, nb_goombas);
			afficher_vies(&etat);
			derniere_mise_a_jour = temps_actuel;
		}

		// Petite pause pour �conomiser le CPU
		Sleep(50);
	}

	//stopMusic();

	// Saisir un nouveau score avec les valeurs finales
	saisir_nouveau_score(nbPiece_actuel, score_actuel);

	// Restaurer la visibilit� du curseur � la fin
	cursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}