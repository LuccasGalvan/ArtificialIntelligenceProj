
#ifndef PROJETO2IIA_ALGORITMO_EVOLUTIVO_H
#define PROJETO2IIA_ALGORITMO_EVOLUTIVO_H

#define MAX_VERT 500 //Numero maximo de vertices

// Estrutura para armazenar parametros
struct info {
    int popsize;            // Tamanho da populacao
    float pm;               // Probabilidade de mutacao
    float pr;               // Probabilidade de recombinacao
    int tsize;              // Tamanho do torneio para selecao do pai da proxima geracao
    int ktam;               // Solucao com ktam vertices a 1
    int numVert;            // Numero dos vertices
    int numLig;             // Numero de ligacoes
    int numGenerations;     // Numero de geracoes
};

// Individuo (solucao)
typedef struct individual chrom, *pchrom;

struct individual {
    int p[MAX_VERT];        // Solucao (vertices dentro da k solucao)
    float fitness;          // Valor da qualidade da solucao
    int valido;             // 1 se for uma solucao valida e 0 se nao for
};

void tournament(pchrom pop, struct info d, pchrom parents);

void roleta(pchrom pop, struct info d, pchrom parents);

void genetic_operators(pchrom parents, struct info d, pchrom offspring);

void crossover(pchrom parents, struct info d, pchrom offspring);

void crossover_uniforme(pchrom parents, struct info d, pchrom offspring);

void mutation(pchrom offspring, struct info d);

void mutacao_por_troca(pchrom offspring, struct info d);

#endif // PROJETO2IIA_ALGORITMO_EVOLUTIVO_H


