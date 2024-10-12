
#include "algoritmo.h"

//#define PROB 0.02
//#define PROB 0.01
#define PROB 0.0005

// Gera um vizinho
// Parametros: solucao actual, vizinho, numero de vertices
//swap two vertices
void gera_vizinho(int a[], int b[], int n) {
    int i, p1, p2;

    for (i = 0; i < n; i++)
        b[i] = a[i];
    // Encontra posicao com valor 0
    do
        p1 = random_l_h(0, n - 1);
    while (b[p1] != 0);
    // Encontra posicao com valor 0
    do
        p2 = random_l_h(0, n - 1);
    while (b[p2] != 1);
    // Troca
    b[p1] = 1;
    b[p2] = 0;
}

//gera um vizinho á distancia 2 (troca dois nos de cada grupo)
//parametros de entrada: solucao actual (a), solucao
void gera_vizinho2(int a[], int b[], int n) {
    int i, p1, p2, p3, p4;

    for (i = 0; i < n; i++)
        b[i] = a[i];
    // Encontra posicao com valor 0
    do
        p1 = random_l_h(0, n - 1);
    while (b[p1] != 0);
    // Encontra posicao com valor 0
    do
        p2 = random_l_h(0, n - 1);
    while (b[p2] != 1);
    // Troca
    b[p1] = 1;
    b[p2] = 0;
    //Encontra aleatoriamente a posicao de um no, que nao seja igual a p2
    do
        p3 = random_l_h(0, n - 1);
    while (b[p3] != 0 || p3 == p2);
    // Encontra aleatoriamente a posicao de um no que nao seja igual a p1
    do
        p4 = random_l_h(0, n - 1);
    while (b[p4] != 1 || p4 == p1);
    //troca os valores dos nos das posicoes encontradas
    b[p3] = 1;
    b[p4] = 0;
}

// Trepa colinas first-choice
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada
int trepa_colinas(int sol[], int *mat, int vert, int num_iter) {
    int *nova_sol, custo, custo_viz, i;

    nova_sol = malloc(sizeof(int) * vert);
    if (nova_sol == NULL) {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    // Avalia solucao inicial
    printf("\nSolucao inicial: \n");
    custo = calcula_fit(sol, mat, vert);
    for (i = 0; i < num_iter; i++) {
        // Gera vizinho 1 e 2
        gera_vizinho(sol, nova_sol, vert);
        //gera_vizinho2(sol, nova_sol, vert);
        // Avalia vizinho
        printf("\nIteracao %d:\n", i);
        custo_viz = calcula_fit(nova_sol, mat, vert);
        escreve_sol(nova_sol, vert);
        printf("\nCusto Solucao: %d", custo_viz);
        // Aceita vizinho se o custo aumenta (problema de maximizacao)
        if (custo_viz < custo) {
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        }
    }
    free(nova_sol);
    return custo;
}

int trepa_colinas_probabilistico(int sol[], int *mat, int vert, int num_iter) {
    int *nova_sol, custo, custo_viz, i;

    nova_sol = malloc(sizeof(int) * vert);
    if (nova_sol == NULL) {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, vert);
    for (i = 0; i < num_iter; i++) {
        // Gera vizinho 1 e 2
        gera_vizinho(sol, nova_sol, vert);
        //gera_vizinho2(sol, nova_sol, vert);
        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, vert);
        // Aceita vizinho se o custo aumentar (problema de maximizacao)
        if (custo_viz >= custo) {
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        } else {
            if (rand_01() < PROB) {
                substitui(sol, nova_sol, vert);
                custo = custo_viz;
            }
        }
    }
    free(nova_sol);
    return custo;
}

