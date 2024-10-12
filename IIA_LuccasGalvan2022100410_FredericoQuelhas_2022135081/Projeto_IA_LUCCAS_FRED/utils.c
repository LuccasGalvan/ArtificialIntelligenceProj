
#include "utils.h"

//Leitura do ficheiro de input
//Recebe: Nome do ficheiro, numero de vertices
// Devolve a matriz com ligacoes
int* init_dados(char *nome, int *n, int *tam) {
    FILE *f;
    int i, j, con, row, col, custo;

    f = fopen(nome, "r");
    if (!f) {
        printf("Erro no acesso ao ficheiro dos dados\n");
        return NULL;
    }
    // Numero de k(tamanho solucao)
    fscanf(f, "k %d", tam);
    // Numero de vertices e ligacoes
    fscanf(f, " p edge %d %d", n, &con);
    // Alocacao dinamica da matriz
    int *p = malloc(sizeof(int) * (*n) * (*n));
    if (!p) {
        printf("Erro na alocacao de memoria\n");
        return NULL;
    }
    // Preenchimento da matriz com Numero arbitrário
    for (i = 0; i < *n; i++)
        for (j = 0; j < *n; j++)
            *(p + (*n) * i + j) = 9999;
    // Preenchimento da matriz com valores do custo respeito as conacoes
    for (i = 0; i < con; i++) {
        fscanf(f, " e %d %d %d", &row, &col, &custo);
        *(p + (*n) * (row - 1) + col - 1) = custo;
        *(p + (*n) * (col - 1) + row - 1) = custo;
        //*(mat+i*vert+j)==1)
    }
    fclose(f);
    return p;
}

// Gera a solucao inicial com k tam valores a 1
// Parametros: Solucao, vertices, tam de k
void gera_sol_inicial(int *sol, int v, int tam) {
    int i, x;
    int cont_um = 0;

    for (i = 0; i < v; i++)
        sol[i] = 0;
    for (i = 0; i < v && cont_um != tam; i++) {
        do
            x = random_l_h(0, v - 1);
        while (sol[x] != 0);
        sol[x] = 1;
        ++cont_um;
    }
}

// Escreve solucao
// Parametros: solucao e numero de vertices
void escreve_sol(int *sol, int vert) {
    int i;
    printf("\nSolucao: ");
    for (i = 0; i < vert; i++)
        if (sol[i] == 1) {
            printf("%2d ", i + 1);
        }
}

// copia vector b para a (tamanho n)
void substitui(int a[], int b[], int n) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = b[i];
}

void init_rand() {
    srand((unsigned) time(NULL));
}

// Devolve valor inteiro aleatorio entre min e max
int random_l_h(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Devolve um valor real aleatorio do intervalo [0, 1]
float rand_01() {
    return ((float) rand()) / RAND_MAX;
}

