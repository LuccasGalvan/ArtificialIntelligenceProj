
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algoritmo_evolutivo.h"
#include "utils_evolutivo.h"
#include "utils.h"

// Leitura dos parametros e dos dados do problema
// Parametros de entrada: Nome do ficheiro e matriz a preencher com os dados dos objectos (peso e valor)
// Parametros de saida: Devolve a estrutura com os parametros
int init_data(char *filename, int *mat, struct info *x) {
    FILE *f;
    int i, j, row, col;

    f = fopen(filename, "rt");
    if (!f) {
        printf("File not found\n");
        return 1;
    }
    // Leitura dos parametros do problema
    fscanf(f, "k %d", &x->ktam);                             // Numero de k(vertices na solucao)
    fscanf(f, " p edge %d %d", &x->numVert, &x->numLig);    // Numero de vertices e ligacoes
    if (x->numVert > MAX_VERT) {
        printf("Number of verts is superior to MAX_VERT\n");
        return 1;
    }
    // Preenchimento da matriz
    int custo;
    for (i = 0; i < x->numVert; i++)
        for (j = 0; j < x->numVert; j++)
            *(mat + (x->numVert) * i + j) = 9999;
    for (i = 0; i < x->numLig; i++) {
        fscanf(f, " e %d %d %d", &row, &col, &custo);
        *(mat + x->numVert * (row - 1) + col - 1) = custo;
        *(mat + x->numVert * (col - 1) + row - 1) = custo;
    }
    fclose(f);
    return 0;
}

// Simula o lancamento de uma moeda, retornando o valor 0 ou 1
int flip() {
    if ((((float) rand()) / RAND_MAX) < 0.5)
        return 0;
    else
        return 1;
}

// Criacao da populacao inicial. O vector e alocado dinamicamente
// Parametro de entrada: Estrutura com parametros do problema
// Parametro de saida: Preenche da estrutura da populacao apenas o vector binario com as ligacoes
pchrom init_pop(struct info d) {
    int i, j;
    pchrom indiv;

    indiv = malloc(sizeof(chrom) * d.popsize);
    if (indiv == NULL) {
        printf("Erro na alocacao de memoria\n");
        return NULL;
    }
    for (i = 0; i < d.popsize; i++) {
        for (j = 0; j < d.numVert; j++)
            indiv[i].p[j] = flip();
    }
    return indiv;
}

// Actualiza a melhor solucao encontrada
// Parametro de entrada: populacao actual (pop), estrutura com parametros (d) e a melhor solucao encontrada ate a geracao imediatamente anterior (best)
// Parametro de saida: a melhor solucao encontrada ate a geracao actual
chrom get_best(pchrom pop, struct info d, chrom best) {
    int i;

    for (i = 0; i < d.popsize; i++) {
        if (best.fitness > pop[i].fitness)
            best = pop[i];
    }
    return best;
}

// Escreve uma solucao na consola
// Parametro de entrada: populacao actual (pop) e estrutura com parametros (d)
void write_best(chrom x, struct info d) {
    int i;

    printf("\nBest individual: %4.1f\n", x.fitness);
    for (i = 0; i < d.numVert; i++)
        if (x.p[i] == 1) {
            printf("%2d ", i + 1);
        }
    putchar('\n');
}

// Devolve um valor inteiro distribuido uniformemente entre min e max
int random_l_h_evolutivo(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Devolve um valor real distribuido uniformemente entre 0 e 1
float rand_01_evolutivo() {
    return ((float) rand()) / RAND_MAX;
}

