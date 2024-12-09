#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "header.h"

int main() {
    setlocale(0, "Portuguese");
    int p = 0,fase_atual = 0, num_personagens = 3; // Índice do personagem selecionado
    Personagem personagem[3];// Array de personagens

    exibir_menu(personagem, num_personagens, &p, &fase_atual);

    return 0;
}

