
#include <stdbool.h>
#include "algoritmo_evolutivo.h"
#include "funcao_evolutivo.h"
#include "utils_evolutivo.h"

#define GENERATION_TC 100
#define PROBGERAVIZ 1.0


// Avaliacao da solucao - metodo de reparacao para ter ktam vertices a 1 na solucao
// Parametros de entrada: solucao (sol), estrutura com parametros (d) e matriz com dados do problema (mat)
// Parametros de saida: Preenche pop com os valores de fitness e de validade para cada solucao
float eval_individual_reparado1(int sol[], struct info d, int *mat, int *v) {
    int total = 0;
    int i, j;
    bool temConexao;

    repara_solucao(sol, d, mat, v);

    //avalia solucao
    for(i=0; i<d.numVert; i++)
        if(sol[i]==1)
        {
            //printf("No: %d\t", i);
            temConexao = false;
            for(j=0; j<d.numVert;j++){
                if(sol[j]==1 && *(mat + (d.numVert) * (j) + i) != 9999){
                    temConexao = true;
                    //printf("Custo: %d\n", *(mat + (vert) * (j) + i));
                    total += *(mat + (d.numVert) * (j) + i);//*(mat+vert*(i+j));//getCustoVertice(a, mat, vert, j);
                }
            }
            if(!temConexao){
                *v = 0;
                total += 9999;
                break;
            }
            //if(&& *(mat+i*vert+j)==1)
            //total++;
        }
    return total / 2;
}

void repara_solucao(int sol[], struct info d, int *mat, int *v) {
    int i;
    float soma = 0;
    // Percorre todos os vertices
    for (i = 0; i < d.numVert; i++) {
        // Verifica se o vertice i esta na solucao
        if (sol[i] == 1) {
            // Actualiza a soma total
            soma++;
        }
    }
    while (soma > d.ktam) {
        //escolhe um vertice aleatoriamente
        do {
            i = random_l_h_evolutivo(0, d.numVert - 1);
        } while (sol[i] == 0);

        //Se esse vertice estiver na solucao a mais, retira-o e ajusta a soma
        sol[i] = 0;
        soma--;
    }
    while (soma < d.ktam) {

        //escolhe um vertice aleatoriamente
        do {
            i = random_l_h_evolutivo(0, d.numVert - 1);
        } while (sol[i] == 1);
        //Se esse vertice estiver na solucao a menos, adiciona-o e ajusta a soma
        sol[i] = 1;
        soma++;
    }
    *v = 1; //solucao valida
}

// Avaliacao da popula��o
// Par�metros de entrada: populacao (pop), estrutura com parametros (d) e matriz com dados do problema (mat)
// Par�metros de sa�da: Preenche pop com os valores de fitness e de validade para cada solu��o
void evaluate(pchrom pop, struct info d, int *mat) {
    int i;

    for (i = 0; i < d.popsize; i++)
        pop[i].fitness = eval_individual_reparado1(pop[i].p, d, mat, &pop[i].valido);
}

void gera_vizinho_hibrido(int sol[], int solViz[], int *mat, int nGenes) {
    int i, menorCustoIn, maiorCustoOut, p1, p2;

    for (i = 0; i < nGenes; i++)
        solViz[i] = sol[i];
    if (rand_01_evolutivo() < PROBGERAVIZ) {
        // escolhe um objeto aleatoriamente
        i = random_l_h_evolutivo(0, nGenes - 1);
        solViz[i] = !solViz[i];
    } else {
        menorCustoIn = MAX_VERT;
        maiorCustoOut = 0;
        for (i = 0; i < nGenes; i++) {
            if (sol[i] == 1 && menorCustoIn > *mat) {
                menorCustoIn = *mat;
                p1 = i;
            }
            if (sol[i] == 0 && maiorCustoOut < *mat) {
                maiorCustoOut = *mat;
                p2 = i;
            }
        }
        solViz[p1] = 0;
        solViz[p2] = 1;
    }
}

void trepa_colinas_hibrido(pchrom pop, struct info d, int *mat) {
    int i, j;
    chrom vizinho;

    for (i = 0; i < d.popsize; i++) {
        for (j = 0; j < GENERATION_TC; j++) {
            gera_vizinho_hibrido(pop[i].p, vizinho.p, mat, d.numVert);
            vizinho.fitness = eval_individual_reparado1(vizinho.p, d, mat, &vizinho.valido);
            if (vizinho.fitness < pop[i].fitness)
                pop[i] = vizinho;
        }
    }
}

