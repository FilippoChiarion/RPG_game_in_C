#ifndef HEADER_H
#define HEADER_H

typedef struct {
    int dano;
    int vida;
    int vida_max;
    float velocidade;
    int level;
    int xp;
    int xp_necessario;
    int mana;
    int mana_max;
    float defesa;
    float chance_critico;
} Personagem;

typedef struct {
    int dano;
    int vida;
    float velocidade;
    char nome[50];
    int fase;
} Boss;

typedef struct{
    int dano;
    int vida;
    char nome[50];
}Inimigo;

// ações
void validarLimites(Personagem *personagem);
void ganharXP(Personagem *personagem, int xp_ganho);
void recuperarMana(Personagem *personagem);
void lutar(int i, Personagem personagem[], Boss boss);
void lutaInimigo(int i, Personagem personagem[], Inimigo inimigo);

// ataques e habilidades especiais
void ataquePython(Personagem *personagem, Boss *boss);
void ataqueJava(Personagem *personagem, Boss *boss);
void ataqueCpp(Personagem *personagem, Boss *boss);

void ultPython(Personagem *personagem, Boss *boss);
void ultJava(Personagem *personagem, Boss *boss);
void ultCpp(Personagem *personagem, Boss *boss);

// pegar item
void pegarItemMilton(Personagem *personagem);
void pegarItemHercules(Personagem *personagem);
void pegarItemBeatriz(Personagem *personagem);
void pegarItemAfranio(Personagem *personagem);
void pegarItemJefferson(Personagem *personagem);


void salvar_jogo(Personagem personagens[], int num_personagens, int p, int fase_atual);
int carregar_jogo(Personagem personagens[], int num_personagens, int *p, int *fase_atual);
void inicializar_jogo(Personagem personagem[], int *total_personagens, int *p, int *fase_atual);
void executar_fases(int *fase_atual, Personagem personagem[], int p);
void exibir_menu(Personagem personagem[], int num_personagens, int *p, int *fase_atual);
void creditos();

// fases
int fase0();
void ler_fase(int n, Personagem personagem[],int p);

#endif
