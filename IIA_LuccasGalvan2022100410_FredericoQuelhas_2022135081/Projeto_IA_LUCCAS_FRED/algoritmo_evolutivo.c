
#include "algoritmo_evolutivo.h"
#include "utils_evolutivo.h"

// Preenche uma estrutura com os progenitores da proxima geracao, de acordo com o resultados do torneio binario (tamanho de torneio: 2)
// Parametros de entrada: populacao actual (pop), estrutura com parametros (d) e populacao de pais a encher
void tournament(pchrom pop, struct info d, pchrom parents) {
    int i, x1, x2;

    // Realiza popsize torneios
    for (i = 0; i < d.popsize; i++) {
        x1 = random_l_h_evolutivo(0, d.popsize - 1);
        do
            x2 = random_l_h_evolutivo(0, d.popsize - 1);
        while (x1 == x2);
        if (pop[x1].fitness < pop[x2].fitness)        // Problema de minimizacao
            parents[i] = pop[x1];
        else
            parents[i] = pop[x2];
    }
}

//Metodo de selecao roleta
// Parametros de entrada: populacao actual (pop), estrutura com parametros (d) e populacao de pais a encher
void roleta(pchrom pop, struct info d, pchrom parents) {
    int i;
    float soma = 0;
    int array[200]; // tamanho maximo populacao
    for (i = 0; i < d.popsize; i++) {
        soma += pop[i].fitness;                    //soma das fitness de cada individuo
    }
    array[0]=pop[0].fitness/soma;                 //guarda o fitness do primeiro individuo no array
    for (i = 1; i < d.popsize; i++) {
        array[i]=array[i-1]+pop[i].fitness/soma; //guarda a qualidade(%) dos fitness dos individuos no array
    }                                            //guarda o valor no array para depois rodar na roleta
    for(int j=0; j < d.popsize; j++) {
        float x = rand_01_evolutivo();          //valor aleatorio 0 e 1
        i = -1;
        do {
            i++;
        } while (array[i] <= x);               //verifica qual foi o individuo escolhido
        parents[j] = pop[i];                   //guarda para ser parent da nova geracao
    }
}

// Operadores geneticos a usar na geracao dos filhos
// Parametros de entrada: estrutura com os pais (parents), estrutura com parametros (d), estrutura que guardar os descendentes (offspring)
void genetic_operators(pchrom parents, struct info d, pchrom offspring) {
    //Recombinacoes/Crossovers
    crossover(parents, d, offspring);                      // Recombinacao com um ponto de corte
    //crossover_uniforme(parents, d, offspring);           // Recombinacao Uniforme
    mutation(offspring, d);                             // Mutacao binaria
    //mutacao_por_troca(offspring, d);                     // Mutação por troca

}

// Preenche o vector descendentes com o resultado das operacoes de recombinacao
// Parametros de entrada: estrutura com os pais (parents), estrutura com parametros (d), estrutura que guardar os descendentes (offspring)
void crossover(pchrom parents, struct info d, pchrom offspring) {
    int i, j, point;

    for (i = 0; i < d.popsize; i += 2) {
        if (rand_01_evolutivo() < d.pr) {
            point = random_l_h_evolutivo(0, d.numVert - 1);
            for (j = 0; j < point; j++) {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i + 1].p[j] = parents[i + 1].p[j];
            }
            for (j = point; j < d.numVert; j++) {
                offspring[i].p[j] = parents[i + 1].p[j];
                offspring[i + 1].p[j] = parents[i].p[j];
            }
        } else {
            offspring[i] = parents[i];
            offspring[i + 1] = parents[i + 1];
        }
    }
}

void crossover_dois_pontos_corte(pchrom parents, struct info d, pchrom offspring) {
    int i, j, point1, point2;

    for (i = 0; i < d.popsize; i += 2) {
        if (rand_01_evolutivo() < d.pr) {
            point1 = random_l_h_evolutivo(0, d.numVert - 1);
            point2 = random_l_h_evolutivo(point1 + 1, d.numVert - 1);
            for (j = 0; j < point1; j++) //Igual aos pais
            {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i + 1].p[j] = parents[i + 1].p[j];
            }
            for (j = point1; j < point2; j++) //Troca
            {
                offspring[i].p[j] = parents[i + 1].p[j];
                offspring[i + 1].p[j] = parents[i].p[j];
            }
            for (j = point2; j < d.numVert; j++) //Igual aos pais
            {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i + 1].p[j] = parents[i + 1].p[j];
            }
        } else {
            offspring[i] = parents[i];
            offspring[i + 1] = parents[i + 1];
        }
    }
}

void crossover_uniforme(pchrom parents, struct info d, pchrom offspring) {
    int i, j;

    for (i = 0; i < d.popsize; i += 2) {
        if (rand_01_evolutivo() < d.pr) {
            for (j = 0; j < d.numVert; j++) {
                if (flip() == 1) {
                    offspring[i].p[j] = parents[i].p[j];
                    offspring[i + 1].p[j] = parents[i + 1].p[j];
                } else {
                    offspring[i].p[j] = parents[i + 1].p[j];
                    offspring[i + 1].p[j] = parents[i].p[j];
                }
            }
        } else {
            offspring[i] = parents[i];
            offspring[i + 1] = parents[i + 1];
        }
    }
}

// Mutacao binaria com varios pontos de mutacao
// Parametros de entrada: estrutura com os descendentes (offspring) e estrutura com parametros (d)
void mutation(pchrom offspring, struct info d) {
    int i, j;

    for (i = 0; i < d.popsize; i++)
        for (j = 0; j < d.numVert; j++)
            if (rand_01_evolutivo() < d.pm)
                offspring[i].p[j] = !(offspring[i].p[j]);
}

void mutacao_por_troca(pchrom offspring, struct info d) {
    int i, pos1, pos2, aux;

    for (i = 0; i < d.popsize; i++) {
        if (rand_01_evolutivo() < d.pm) {
            do
                pos1 = random_l_h_evolutivo(0, d.numVert - 1);
            while (offspring[i].p[pos1] == 1);
            do
                pos2 = random_l_h_evolutivo(0, d.numVert - 1);
            while (offspring[i].p[pos2] == 0);
            aux = offspring[i].p[pos1] = offspring[i].p[pos2];
            offspring[i].p[pos2] = aux;
        }
    }
}

