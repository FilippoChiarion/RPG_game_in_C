#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"




void salvar_jogo(Personagem personagem[], int num_personagens, int p, int fase_atual) {
    FILE *arquivo = fopen("savegame.txt", "w");


    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar o jogo.\n");
        return;
    }

    // Salva o personagem atual e suas informações
    fprintf(arquivo, "Personagem Atual: %d\n", p + 1);
    fprintf(arquivo, "Dano: %d\n", personagem[p].dano);
    fprintf(arquivo, "Vida: %d\n", personagem[p].vida);
    fprintf(arquivo, "Vida Máxima: %d\n", personagem[p].vida_max);
    fprintf(arquivo, "Velocidade: %.2f\n", personagem[p].velocidade);
    fprintf(arquivo, "Level: %d\n", personagem[p].level);
    fprintf(arquivo, "XP: %d\n", personagem[p].xp);
    fprintf(arquivo, "XP Necessário: %d\n", personagem[p].xp_necessario);
    fprintf(arquivo, "Mana: %d\n", personagem[p].mana);
    fprintf(arquivo, "Mana Máxima: %d\n", personagem[p].mana_max);
    fprintf(arquivo, "Defesa: %.2f\n", personagem[p].defesa);
    fprintf(arquivo, "Chance Crítico: %.2f\n", personagem[p].chance_critico);

    // Salva a fase atual
    fprintf(arquivo, "Fase Atual: %d\n", fase_atual);

    // Fecha o arquivo
    fclose(arquivo);

    printf("Jogo salvo com sucesso!\n");
}


int carregar_jogo(Personagem personagem[], int num_personagens, int *p, int *fase_atual) {
    FILE *arquivo = fopen("savegame.txt", "r");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de salvamento.\n");
        return 0; // Retorna 0 indicando que não foi possível carregar o jogo
    }

    // Lê os dados do arquivo e carrega para o personagem
    if (fscanf(arquivo, "Personagem Atual: %d\n", p) != 1) {
        fclose(arquivo);
        return 0;
    }
    (*p)--;  // Ajusta o índice para ser 0-based

    fscanf(arquivo, "Dano: %d\n", &personagem[*p].dano);
    fscanf(arquivo, "Vida: %d\n", &personagem[*p].vida);
    fscanf(arquivo, "Vida Máxima: %d\n", &personagem[*p].vida_max);
    fscanf(arquivo, "Velocidade: %f\n", &personagem[*p].velocidade);
    fscanf(arquivo, "Level: %d\n", &personagem[*p].level);
    fscanf(arquivo, "XP: %d\n", &personagem[*p].xp);
    fscanf(arquivo, "XP Necessário: %d\n", &personagem[*p].xp_necessario);
    fscanf(arquivo, "Mana: %d\n", &personagem[*p].mana);
    fscanf(arquivo, "Mana Máxima: %d\n", &personagem[*p].mana_max);
    fscanf(arquivo, "Defesa: %f\n", &personagem[*p].defesa);
    fscanf(arquivo, "Chance Crítico: %f\n", &personagem[*p].chance_critico);
    fscanf(arquivo, "Fase Atual: %d\n", fase_atual);

    fclose(arquivo);
    return 1; // Sucesso ao carregar o jogo
}



void exibir_menu(Personagem personagem[], int num_personagens, int *p, int *fase_atual) {
    setlocale(0, "Portuguese");
    int opcao;

    do {
        printf("\nMenu Principal:\n");
        printf("1. Novo jogo\n");
        printf("2. Continuar\n");
        printf("3. Estatistica\n");
        printf("4. Sair\n");

        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Iniciar novo jogo
                printf("Inicializando jogo...\n");
                inicializar_jogo(personagem, &num_personagens, p, fase_atual);  // Corrigido: passa p diretamente
                executar_fases(fase_atual, personagem, *p);  // Inicia as fases
                break;

            case 2: // Continuar jogo
                if (carregar_jogo(personagem, num_personagens, p, fase_atual)) {
                    printf("\nJogo carregado com sucesso.\n");
                    executar_fases(fase_atual, personagem, *p); // Continua a partir da fase salva
                } else {
                    printf("\nNenhum jogo salvo encontrado. Iniciando um novo jogo.\n");
                    inicializar_jogo(personagem, &num_personagens, p, fase_atual); // Inicializa novo jogo
                }
                break;

            case 3: // Exibir estatísticas
                if (carregar_jogo(personagem, num_personagens, p, fase_atual)) {
                    // Exibe as estatísticas do personagem carregado
                    printf("\nEstatísticas do Personagem %d:\n", *p + 1);
                    printf("Dano: %d\n", personagem[*p].dano);
                    printf("Vida: %d/%d\n", personagem[*p].vida, personagem[*p].vida_max);
                    printf("Velocidade: %.1f\n", personagem[*p].velocidade);
                    printf("Level: %d\n", personagem[*p].level);
                    printf("XP: %d/%d\n", personagem[*p].xp, personagem[*p].xp_necessario);
                    printf("Mana: %d/%d\n", personagem[*p].mana, personagem[*p].mana_max);
                    printf("Defesa: %.1f\n", personagem[*p].defesa);
                    printf("Chance de Critico: %.1f%%\n", personagem[*p].chance_critico * 100);
                    printf("--------------------------------------\n");
                } else {
                    printf("Não tem dados salvos.\n");
                }
                break;

            case 4: // Sair
                printf("Saindo do jogo. Até mais!\n");
                exit(0);
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (1);
}


void executar_fases(int *fase_atual, Personagem personagem[], int p) {
    printf("\nIniciando as fases...\n");
    for (int i = *fase_atual + 1; i <= 4; i++) {
        ler_fase(i, personagem, p);  // Função que carrega e executa a fase
        *fase_atual = i;  // Atualiza a fase atual
    }

    // Após completar as fases normais, enfrentar o chefe
    if (*fase_atual == 4) {
        printf("\nPreparando para a batalha final!\n");
        fase5(p, personagem);
        *fase_atual = 5;
        creditos();
        exit(0);
    }

    printf("\nVocê completou todas as fases! Parabéns!\n");
}
void inicializar_jogo(Personagem personagem[], int *total_personagens, int *p, int *fase_atual) {
    fase0();  // Chama a função de introdução

    // Reinicializa os personagens
    personagem[0] = (Personagem){.dano = 40, .vida = 15000, .vida_max = 15000, .velocidade = 1.0f, .level = 1, .xp = 0, .xp_necessario = 50, .mana = 30, .mana_max = 30, .defesa = 0.0f, .chance_critico = 0.0f};
    personagem[1] = (Personagem){.dano = 50, .vida = 140, .vida_max = 140, .velocidade = 1.2f, .level = 1, .xp = 0, .xp_necessario = 50, .mana = 30, .mana_max = 30, .defesa = 0.0f, .chance_critico = 0.0f};
    personagem[2] = (Personagem){.dano = 45, .vida = 160, .vida_max = 160, .velocidade = 1.1f, .level = 1, .xp = 0, .xp_necessario = 50, .mana = 30, .mana_max = 30, .defesa = 0.0f, .chance_critico = 0.0f};

    printf("\nEscolha sua habilidade:\n");
    printf("1. Python (Alta vida, dano medio)\n");
    printf("2. Java (Alta velocidade, dano alto)\n");
    printf("3. C++ (Alta defesa, vida maior)\n");
    printf("Digite o numero do personagem: ");
    scanf("%d", p);

    // Valida a entrada
    if (*p < 1 || *p > 3) {
        printf("\nOpção invalida. Selecionando o Guerreiro como padrão.\n");
        *p = 1;
    }

    *p -= 1;  // Ajusta o índice do personagem para 0 baseado na escolha

    // Configura a fase inicial (fazendo com que o arquivo de save registre isso)
    *fase_atual = 0;

    // Sobrescreve o arquivo de salvamento, garantindo que o progresso seja limpo
    salvar_jogo(personagem, *total_personagens, *p, *fase_atual);
}






// Função para validar vida e mana máximas
void validarLimites(Personagem *personagem) {
    if (personagem->vida > personagem->vida_max) {
        personagem->vida = personagem->vida_max;
    }
    if (personagem->mana > personagem->mana_max) {
        personagem->mana = personagem->mana_max;
    }
}

// ganhar XP
void ganharXP(Personagem *personagem, int xp_ganho) {
    personagem->xp += xp_ganho;
    printf("Voce ganhou %d de XP!\n", xp_ganho);

    while (personagem->xp >= personagem->xp_necessario) {
        personagem->xp -= personagem->xp_necessario;

        int dano_antigo = personagem->dano;
        int vida_antiga = personagem->vida;
        int mana_max_antiga = personagem->mana_max;

        personagem->level++;
        personagem->dano += 5;
        personagem->vida += 20;
        personagem->vida_max += 20;
        personagem->mana += 5;
        personagem->mana_max += 5;
        personagem->xp_necessario += 50;

        validarLimites(personagem);


        printf("\n*** PARABENS! Voce subiu para o nivel %d! ***\n", personagem->level);
        printf("Atributos aumentados:\n");
        printf("- Dano: %d -> %d (+%d)\n", dano_antigo, personagem->dano, personagem->dano - dano_antigo);
        printf("- Vida: %d -> %d (+%d)\n", vida_antiga, personagem->vida, personagem->vida - vida_antiga);
        printf("- Mana: %d -> %d (+%d)\n", personagem->mana-5, personagem->mana, 5);
        printf("- Mana Maxima: %d -> %d (+%d)\n", mana_max_antiga, personagem->mana_max, personagem->mana_max - mana_max_antiga);
        printf("XP necessario para o proximo nivel: %d\n", personagem->xp_necessario);
        printf("XP atual: %d\n", personagem->xp);
    }
}

// recuperar mana
void recuperarMana(Personagem *personagem) {
    int mana_recuperada = 10;
    personagem->mana += mana_recuperada;
    validarLimites(personagem);
    printf("Você recuperou %d mana. Mana atual: %d/%d.\n", mana_recuperada, personagem->mana, personagem->mana_max);
}

// ataque
void ataquePython(Personagem *personagem, Boss *boss) {
    int dano = personagem->dano;
    float chance = (float)rand() / RAND_MAX;  // Gera um número entre 0 e 1

    if (chance < personagem->chance_critico) {
        dano *= 2;  // Dano crítico é o dobro do dano normal
        printf("Ataque critico! ");
    }

    printf("Python ataca com uma cobra venenosa, causando %d de dano!\n", dano);
    boss->vida -= dano;
}

void ataqueJava(Personagem *personagem, Boss *boss) {
    int dano = personagem->dano;
    float chance = (float)rand() / RAND_MAX;

    if (chance < personagem->chance_critico) {
        dano *= 2;
        printf("Ataque crítico! ");
    }

    printf("Java ataca com sua picareta de Minecraft, causando %d de dano!\n", dano);
    boss->vida -= dano;
}

void ataqueCpp(Personagem *personagem, Boss *boss) {
    int dano = personagem->dano;
    float chance = (float)rand() / RAND_MAX;

    if (chance < personagem->chance_critico) {
        dano *= 2;
        printf("Ataque critico! ");
    }

    printf("C++ ataca com um ponteiro, causando %d de dano!\n", dano);
    boss->vida -= dano;
}


// Ultimate
void ultPython(Personagem *personagem, Boss *boss) {
    printf("Python usa Inteligencia Artificial, causando %d de dano!\n", personagem->dano * 2);
    boss->vida -= personagem->dano * 2;
    personagem->mana -= 20;
}

void ultJava(Personagem *personagem, Boss *boss) {
    printf("Java usa Encapsulamento Explosivo, causando %d de dano!\n", personagem->dano * 2);
    boss->vida -= personagem->dano * 2;
    personagem->mana -= 20;
}

void ultCpp(Personagem *personagem, Boss *boss) {
    printf("C++ usa Overflow, causando %d de dano!\n", personagem->dano * 3);
    boss->vida -= personagem->dano * 3;
    personagem->mana -= 20;
}

void pegarItem(int codItem, Personagem *personagem) {
    switch (codItem)    {
    case 1:
        personagem->defesa += 0.20f;  // Aumento de 20% na defesa
        printf("Voce pegou o item de Milton: Div com espacamento perfeito.\nSua defesa aumentou em 20%%!\n");
        break;
    case 2:
        personagem->dano += 20;  // Aumento de 20 no dano.
        printf("Voce pegou o item de Hercules: Teorema da Forca.\nAumento no dano em 20!\n");
        break;
    case 3:
        personagem->chance_critico += 0.2f;  // Chance de cr�tico de 20%
        printf("Voce pegou o item de Beatriz: Bit de Overflow.\nSua chance de critico aumentou para 20%%!\n");
        break;
    case 4:
        personagem->vida = personagem->vida_max;
        personagem->mana = personagem->mana_max;
        printf("Voce pegou o item de Afranio: Calice da Sabedoria.\nVocê restaurou completamente sua vida e sua mana!\n");

    default:
        break;
    }

}

// Função de luta
void lutar(int i, Personagem personagem[], Boss boss) {
    Boss *bossP = (Boss *)malloc(sizeof(Boss));
    if(bossP == NULL){
        printf("Erro ao alocar memoria para o boss.\n");
        exit(1);
    }
    *bossP = boss;

    while (personagem[i].vida > 0 && boss.vida > 0) {
        validarLimites(&personagem[i]);

        printf("\n--- SUA VEZ ---\n");
        printf("Sua vida: %d | Sua mana: %d/%d (Mana necessaria para ult: 20)\n",
               personagem[i].vida, personagem[i].mana, personagem[i].mana_max);
        printf("Vida de %s: %d\n", boss.nome, boss.vida);

        int escolha;
        do {
            printf("\nEscolha sua acao:\n");
            printf("1 - Atacar\n");
            printf("2 - Recuperar Mana\n");
            printf("3 - Usar Habilidade Especial\n");
            scanf("%d", &escolha);

            if (escolha == 1) {
                if (i == 0) ataquePython(&personagem[i], &boss);
                else if (i == 1) ataqueJava(&personagem[i], &boss);
                else if (i == 2) ataqueCpp(&personagem[i], &boss);
                break;
            } else if (escolha == 2) {
                recuperarMana(&personagem[i]);
                break;
            } else if (escolha == 3) {
                if (personagem[i].mana >= 20) {
                    if (i == 0) ultPython(&personagem[i], &boss);
                    else if (i == 1) ultJava(&personagem[i], &boss);
                    else if (i == 2) ultCpp(&personagem[i], &boss);
                    break;
                } else {
                    printf("Mana insuficiente! Escolha outra opcao.\n");
                }
            } else {
                printf("Opção invalida! Escolha novamente.\n");
            }
        } while (1);

        if (boss.vida <= 0) {
            printf("Voce derrotou %s!\n", boss.nome);
            ganharXP(&personagem[i], 100);
            personagem[i].vida += 50;
            personagem[i].mana += 20;

            validarLimites(&personagem[i]);


            printf("O Boss dropou uma pocao magica, voce usou ela e recuperou 50 de vida e 20 de mana.\nSua nova vida e: %d\n", personagem[i].vida);
            pegarItem(boss.fase, &personagem[i]);
            break;
        }
        printf("\n--- VEZ DE %s ---\n", boss.nome);


        // Cálculo de dano reduzido
        int dano_final = boss.dano * (1.0f - personagem[i].defesa);
        printf("%s atacou e causou %d de dano!\n", boss.nome, dano_final, boss.dano);
        personagem[i].vida -= dano_final;

        if (personagem[i].vida <= 0) {
            printf("Voce foi derrotado por %s...\n", boss.nome);
        }
    }
    free(bossP);

}


void lutaInimigo(int i, Personagem personagem[], Inimigo inimigos) {

    Inimigo *inimigop = (Inimigo *)malloc(sizeof(Inimigo));
    if(inimigop == NULL){
        printf("Erro ao alocar memoria para o inimigo.\n");
        exit(1);
    }
    *inimigop = inimigos;

    int escolha;
    printf("Voce encontrou o %s! Prepare-se para a batalha.\n", inimigos.nome);

    validarLimites(&personagem[i]);

    while (personagem[i].vida > 0 && inimigos.vida > 0) {
        printf("\n--- SUA VEZ ---\n");
        printf("Sua vida: %d/%d\n", personagem[i].vida, personagem[i].vida_max);
        printf("Vida do %s: %d\n", inimigos.nome, inimigos.vida);

        // Solicitar escolha de ação do jogador
        do {
            printf("\nEscolha sua acao:\n");
            printf("1 - Ataque Basico\n");
            printf("2 - Fazer nada kk\n");
            printf("Escolha: ");
            scanf("%d", &escolha);

            if (escolha != 1 && escolha != 2) {
                printf("Escolha invalida! Por favor, escolha entre 1 e 2.\n");
            }
        } while (escolha != 1 && escolha != 2);

        switch (escolha) {
            case 1:
                printf("Voce ataca o %s, causando %d de dano!\n", inimigos.nome, personagem[i].dano);
                inimigos.vida -= personagem[i].dano;

                if (inimigos.vida <= 0) {
                    printf("Voce derrotou o %s!\n", inimigos.nome);
                    ganharXP(&personagem[i], 50);
                    return;
                }
                break;

            case 2:
                printf("Voce nao fez nada...\n");
                break;

            default:
                printf("Escolha invalida! Voce perdeu sua vez...\n");
                break;
        }

        // Turno do inimigo
        printf("\n--- VEZ DO %s ---\n", inimigos.nome);
        printf("O %s ataca e causa %d de dano!\n", inimigos.nome, inimigos.dano);
        personagem[i].vida -= inimigos.dano;

        if (personagem[i].vida <= 0) {
            printf("Voce foi derrotado pelo %s...\n", inimigos.nome);
            return;
        }
    }
    free(inimigop);

}


void ler_fase(int n, Personagem personagem[],int p) {
    FILE *fase;
    Boss *boss = (Boss *)malloc(sizeof(Boss)); // Alocação dinâmica
    Inimigo *inimigos = (Inimigo *)malloc(sizeof(Inimigo));
    if (boss == NULL) {
        printf("Erro ao alocar memoria para o boss.\n");
        exit(1);
    }
    int boss_encontrado=0, inimigo_encontrado=0;
    char nome_arquivo[100];
    char linha[200]; //le linha
    char enter;

    sprintf(nome_arquivo, "fase%d.txt", n);
    fase = fopen(nome_arquivo, "r");
    if (fase == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nome_arquivo);
        free(boss);
        exit(1);
    }

    while (fgets(linha, sizeof(linha), fase) != NULL) {
        // Verifica se a linha contém "---LUTA INIMIGO---"
        if (strstr(linha, "---LUTA INIMIGO---") != NULL){ //inimigo_encontrado = 1;
            fgets(linha,sizeof(linha), fase);
            sscanf(linha, "%d %d %s", &inimigos->dano, &inimigos->vida, inimigos->nome);
            lutaInimigo(p, personagem, *inimigos);
            fgets(linha, sizeof(linha), fase);
        }


        // Verifica se a linha contém "---LUTA BOSS---"
        else if (strstr(linha, "---LUTA BOSS---") != NULL){
            fgets(linha,sizeof(linha),fase);
            sscanf(linha, "%d %d %s", &boss->dano, &boss->vida, boss->nome);
            printf("Boss carregado: %s (Dano: %d, Vida: %d)\n", boss->nome, boss->dano, boss->vida);
            boss->fase = n;
            lutar(p, personagem, *boss);
            fgets(linha, sizeof(linha), fase);
        }


        if(strstr(linha, "---LUTA INIMIGO---") == NULL){
            if(strstr(linha, "---LUTA BOSS---") == NULL)printf("%s",linha);
        }
    }
    salvar_jogo(personagem, 3, p, n);
    printf("\nPressione Enter para continuar para a proxima fase ...\n");
    getchar();
    enter = getchar();
    if(enter == '\n')system("cls");
    fclose(fase);
}

int fase0(){
    int p ;
    char linha[200];
    char enter, sair;
    FILE *fase;
    fase = fopen("fase0.txt","r");
    while(fgets(linha, sizeof(linha), fase) != NULL){
        printf("%s",linha);
        if(strstr(linha,"escolha:")!= NULL){
            scanf("%d",&p);
            switch(p){
                case 0:printf("Voce escolheu Python!\n");
                break;
                case 1:printf("Voce escolheu Java!\n");
                break;
                case 2:printf("Voce escolheu C++!\n");
                break;
            }
        }
    }


    printf("\nPressione Enter para continuar para a proxima fase...\n");
    getchar();
    enter = getchar();
    sair == getchar();
    if(enter == '\n')system("cls");
    fclose(fase);

    return p;

}


void creditos() {
    printf("\n\n");
    printf("=========================================\n");
    printf("|\t        CREDITOS DO JOGO      \t|\n");
    printf("=========================================\n");
    printf("|\tDesenvolvimento:              \t|\n");
    printf("|\t    Filippo Chiarion          \t|\n");
    printf("|\t    Gabriel Alves Santana     \t|\n");
    printf("|\t    Gabriel Sales             \t|\n");
    printf("|\t                              \t|\n");
    printf("|\tDesign e Graficos:            \t|\n");
    printf("|\t    Gabriel Sales             \t|\n");
    printf("|\t    Felipe Fernando           \t|\n");
    printf("|\t                              \t|\n");
    printf("|\tTestes e Feedback:            \t|\n");
    printf("|\t    Fernando Albernaz         \t|\n");
    printf("|\t    Gabriel Lucas             \t|\n");
    printf("|\t                              \t|\n");
    printf("=========================================\n");
    printf("|\tObrigado por jogar!            \t|\n");
    printf("=========================================\n");
}


int fase5(int p, Personagem personagem[]) {
    int opc;
    Boss *boss = (Boss *)malloc(sizeof(Boss));
    char linha[200];
    FILE *fase;

    // Abrindo o arquivo
    fase = fopen("fase5.txt", "r");
    if (!fase) {
        printf("Erro ao abrir o arquivo\n");
        return -1;
    }

    while (fgets(linha, sizeof(linha), fase) != NULL) {
        if (strstr(linha, "decisao.") != NULL){

            do{printf("\nEscolha:\n");
            printf("1 - Virar a pagina\n");
            printf("2 - Fechar o livro\n");
            scanf("%d",&opc);
            }while(opc!=1 && opc!=2);
                if(opc==1){
                    while (fgets(linha, sizeof(linha), fase) != NULL){
                        if(strstr(linha,"AQUI ACABOU") != NULL) fclose(fase);
                        else printf("%s",linha);
                }
            }
        }
        else if(opc==2){
            for (int i = 0; i < 54; i++) {
            fgets(linha, sizeof(linha), fase);
        }
            while (fgets(linha, sizeof(linha), fase) != NULL){
                    if(strstr(linha,"---LUTA BOSS---") != NULL){
                        fgets(linha,sizeof(linha),fase);
                        sscanf(linha, "%d %d %s", &boss->dano, &boss->vida, boss->nome);
                        lutar(p, personagem, *boss);
                    }
                    else printf("%s",linha);
            }

        }else printf("%s", linha);
    }
    fclose(fase);
    salvar_jogo(personagem, 3, p, 5);
}
