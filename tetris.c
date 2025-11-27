#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILA_CAPACIDADE 5
#define PILHA_CAPACIDADE 3

// Estrutura para representar uma peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura para a fila circular
typedef struct {
    Peca elementos[FILA_CAPACIDADE];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

// Estrutura para a pilha
typedef struct {
    Peca elementos[PILHA_CAPACIDADE];
    int topo;
    int tamanho;
} Pilha;

// Protótipos das funções
void inicializarFila(FilaCircular *fila);
void inicializarPilha(Pilha *pilha);
Peca gerarPeca(int id);
int filaVazia(FilaCircular *fila);
int filaCheia(FilaCircular *fila);
int pilhaVazia(Pilha *pilha);
int pilhaCheia(Pilha *pilha);
void enfileirar(FilaCircular *fila, Peca peca);
Peca desenfileirar(FilaCircular *fila);
void empilhar(Pilha *pilha, Peca peca);
Peca desempilhar(Pilha *pilha);
void jogarPeca(FilaCircular *fila, int *proximoId);
void reservarPeca(FilaCircular *fila, Pilha *pilha, int *proximoId);
void usarPecaReservada(Pilha *pilha);
void trocarPeca(FilaCircular *fila, Pilha *pilha);
void trocaMultipla(FilaCircular *fila, Pilha *pilha);
void exibirEstado(FilaCircular *fila, Pilha *pilha);
void mostrarMenu();

// Variável global para controle de IDs
int proximoId = 0;

int main() {
    FilaCircular fila;
    Pilha pilha;
    
    // Inicializar estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    // Preencher a fila inicial com peças
    for (int i = 0; i < FILA_CAPACIDADE; i++) {
        enfileirar(&fila, gerarPeca(proximoId++));
    }
    
    int opcao;
    
    printf("=== GERENCIADOR DE PEÇAS ===\n");
    printf("Fila circular: %d peças | Pilha: %d peças\n\n", FILA_CAPACIDADE, PILHA_CAPACIDADE);
    
    do {
        exibirEstado(&fila, &pilha);
        mostrarMenu();
        
        printf("\nOpção escolhida: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                jogarPeca(&fila, &proximoId);
                break;
            case 2:
                reservarPeca(&fila, &pilha, &proximoId);
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarPeca(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        
        printf("\n");
        
    } while (opcao != 0);
    
    return 0;
}

// Inicializa a fila circular
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
    pilha->tamanho = 0;
}

// Gera uma peça aleatória
Peca gerarPeca(int id) {
    Peca peca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    // Usar o ID como semente para variedade
    srand(time(NULL) + id);
    peca.nome = tipos[rand() % 4];
    peca.id = id;
    
    return peca;
}

// Verifica se a fila está vazia
int filaVazia(FilaCircular *fila) {
    return fila->tamanho == 0;
}

// Verifica se a fila está cheia
int filaCheia(FilaCircular *fila) {
    return fila->tamanho == FILA_CAPACIDADE;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *pilha) {
    return pilha->topo == PILHA_CAPACIDADE - 1;
}

// Adiciona uma peça na fila
void enfileirar(FilaCircular *fila, Peca peca) {
    if (filaCheia(fila)) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    
    fila->tras = (fila->tras + 1) % FILA_CAPACIDADE;
    fila->elementos[fila->tras] = peca;
    fila->tamanho++;
}

// Remove uma peça da fila
Peca desenfileirar(FilaCircular *fila) {
    if (filaVazia(fila)) {
        printf("Erro: Fila vazia!\n");
        Peca vazia = {' ', -1};
        return vazia;
    }
    
    Peca peca = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % FILA_CAPACIDADE;
    fila->tamanho--;
    
    return peca;
}

// Adiciona uma peça na pilha
void empilhar(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf("Erro: Pilha cheia!\n");
        return;
    }
    
    pilha->elementos[++(pilha->topo)] = peca;
    pilha->tamanho++;
}

// Remove uma peça da pilha
Peca desempilhar(Pilha *pilha) {
    if (pilhaVazia(pilha)) {
        printf("Erro: Pilha vazia!\n");
        Peca vazia = {' ', -1};
        return vazia;
    }
    
    pilha->tamanho--;
    return pilha->elementos[(pilha->topo)--];
}

// Joga a peça da frente da fila
void jogarPeca(FilaCircular *fila, int *proximoId) {
    if (filaVazia(fila)) {
        printf("Erro: Não há peças para jogar!\n");
        return;
    }
    
    Peca pecaJogada = desenfileirar(fila);
    printf("Ação: Peça [%c %d] jogada.\n", pecaJogada.nome, pecaJogada.id);
    
    // Gerar nova peça para manter a fila cheia
    if (!filaCheia(fila)) {
        enfileirar(fila, gerarPeca((*proximoId)++));
        printf("Nova peça gerada automaticamente.\n");
    }
}

// Reserva a peça da frente da fila para a pilha
void reservarPeca(FilaCircular *fila, Pilha *pilha, int *proximoId) {
    if (filaVazia(fila)) {
        printf("Erro: Não há peças na fila para reservar!\n");
        return;
    }
    
    if (pilhaCheia(pilha)) {
        printf("Erro: Pilha de reserva cheia!\n");
        return;
    }
    
    Peca pecaReservada = desenfileirar(fila);
    empilhar(pilha, pecaReservada);
    printf("Ação: Peça [%c %d] movida para a pilha de reserva.\n", 
           pecaReservada.nome, pecaReservada.id);
    
    // Gerar nova peça para manter a fila cheia
    if (!filaCheia(fila)) {
        enfileirar(fila, gerarPeca((*proximoId)++));
        printf("Nova peça gerada automaticamente.\n");
    }
}

// Usa a peça do topo da pilha
void usarPecaReservada(Pilha *pilha) {
    if (pilhaVazia(pilha)) {
        printf("Erro: Não há peças reservadas para usar!\n");
        return;
    }
    
    Peca pecaUsada = desempilhar(pilha);
    printf("Ação: Peça reservada [%c %d] usada.\n", pecaUsada.nome, pecaUsada.id);
}

// Troca a peça da frente da fila com o topo da pilha
void trocarPeca(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("Erro: É necessário ter peças na fila E na pilha para trocar!\n");
        return;
    }
    
    // Remover peças temporariamente
    Peca pecaFila = desenfileirar(fila);
    Peca pecaPilha = desempilhar(pilha);
    
    // Trocar e recolocar
    enfileirar(fila, pecaPilha);
    empilhar(pilha, pecaFila);
    
    printf("Ação: Troca realizada entre [%c %d] (fila) e [%c %d] (pilha).\n",
           pecaFila.nome, pecaFila.id, pecaPilha.nome, pecaPilha.id);
}

// Troca múltipla entre as 3 primeiras peças da fila e as 3 da pilha
void trocaMultipla(FilaCircular *fila, Pilha *pilha) {
    if (fila->tamanho < 3 || pilha->tamanho < 3) {
        printf("Erro: É necessário ter pelo menos 3 peças na fila E na pilha!\n");
        return;
    }
    
    printf("Ação: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
    
    // Arrays temporários para armazenar as peças
    Peca tempFila[3], tempPilha[3];
    
    // Remover 3 peças da fila
    for (int i = 0; i < 3; i++) {
        tempFila[i] = desenfileirar(fila);
    }
    
    // Remover 3 peças da pilha
    for (int i = 0; i < 3; i++) {
        tempPilha[i] = desempilhar(pilha);
    }
    
    // Colocar as peças da pilha na fila (na ordem original)
    for (int i = 0; i < 3; i++) {
        enfileirar(fila, tempPilha[i]);
    }
    
    // Colocar as peças da fila na pilha (na ordem inversa para manter LIFO)
    for (int i = 2; i >= 0; i--) {
        empilhar(pilha, tempFila[i]);
    }
}

// Exibe o estado atual da fila e pilha
void exibirEstado(FilaCircular *fila, Pilha *pilha) {
    printf("=== ESTADO ATUAL ===\n");
    
    // Exibir fila
    printf("Fila de peças\t");
    if (filaVazia(fila)) {
        printf("[Vazia]");
    } else {
        int indice = fila->frente;
        for (int i = 0; i < fila->tamanho; i++) {
            printf("[%c %d] ", fila->elementos[indice].nome, fila->elementos[indice].id);
            indice = (indice + 1) % FILA_CAPACIDADE;
        }
    }
    printf("\n");
    
    // Exibir pilha
    printf("Pilha de reserva\t");
    if (pilhaVazia(pilha)) {
        printf("[Vazia]");
    } else {
        printf("(Topo -> base): ");
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->elementos[i].nome, pilha->elementos[i].id);
        }
    }
    printf("\n");
}

// Mostra o menu de opções
void mostrarMenu() {
    printf("\n=== OPÇÕES DISPONÍVEIS ===\n");
    printf("Código\tAção\n");
    printf("1\tJogar peça da frente da fila\n");
    printf("2\tEnviar peça da fila para a pilha de reserva\n");
    printf("3\tUsar peça da pilha de reserva\n");
    printf("4\tTrocar peça da frente da fila com o topo da pilha\n");
    printf("5\tTrocar os 3 primeiros da fila com as 3 peças da pilha\n");
    printf("0\tSair\n");
}
