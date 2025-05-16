void scoreboard(int nbPiece, int score) {
    // Supprimer la v�rification des anciennes valeurs pour que le scoreboard soit toujours affich�

    // D�terminer la largeur n�cessaire pour le score
    int score_length = 0;
    int temp_score = score;
    do {
        score_length++;
        temp_score /= 10;
    } while (temp_score > 0);

    // Minimum 5 caract�res pour le score
    if (score_length < 5) {
        score_length = 5;
    }

    // Format: "# Pieces : XXX     Score : YYYYY     #"
    // 34 caract�res de base + la diff�rence si score_length > 5
    int width = 34;
    if (score_length > 5) {
        width += (score_length - 5);
    }

    // Positionner le curseur en haut � gauche
    printf("\033[1;1H");

    // Ligne sup�rieure de #
    for (int i = 0; i < width; i++) {
        printf("#");
    }
    printf("\n");

    // Ligne vide
    printf("#");
    for (int i = 0; i < width - 2; i++) {
        printf(" ");
    }
    printf("#\n");

    // Ligne avec les informations
    printf("# Pieces : %3d     Score : %*d #\n", nbPiece, score_length, score);

    // Ligne vide
    printf("#");
    for (int i = 0; i < width - 2; i++) {
        printf(" ");
    }
    printf("#\n");

    // Ligne inf�rieure de #
    for (int i = 0; i < width; i++) {
        printf("#");
    }
    printf("\n");
}