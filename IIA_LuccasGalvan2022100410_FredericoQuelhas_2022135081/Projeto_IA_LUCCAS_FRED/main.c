#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo.h"
#include "algoritmo_evolutivo.h"
#include "utils.h"
#include "utils_evolutivo.h"
#include "funcao_evolutivo.h"

#define DEFAULT_RUNS 30

int algoritmo_trepa_colinas(int argc, char *argv[]) {
    char nome_ficheiro[100];
    int vert, num_iter, k, runs, custo, best_custo, sol_tam;
    int *grafo = NULL, *sol, *best;
    float mbf = 0.0;
    if (argc == 3) {
        runs = atoi(argv[2]);
        strcpy(nome_ficheiro, argv[1]);
    } else if (argc == 2) {
        runs = DEFAULT_RUNS;
        strcpy(nome_ficheiro, argv[1]);
    } else {
        runs = DEFAULT_RUNS;
        printf("Nome do Ficheiro: ");
        fflush(stdin);
        gets(nome_ficheiro);
    }
    if (runs <= 0)
        return 0;

    printf("Introduza o Numero de Itera��es: ");
    scanf(" %d", &num_iter);

    init_rand();
    // Preenche matriz de adjacencias
    grafo = init_dados(nome_ficheiro, &vert, &sol_tam);
    if(grafo == NULL){
        return 1;
    }
    sol = malloc(sizeof(int) * vert);
    best = malloc(sizeof(int) * vert);
    if (sol == NULL || best == NULL) {
        printf("Erro na alocacao de memoria");
        free(grafo);
        if(sol == NULL)
            free(best);
        else
            free(sol);
        return 1;
    }
    for (k = 0; k < runs; k++) {
        // Gerar solucao inicial
        gera_sol_inicial(sol, vert, sol_tam);
        escreve_sol(sol, vert);
        // Trepa colinas - First choice
        custo = trepa_colinas(sol, grafo, vert, num_iter);
        //Trepa colinas probabilistico
        //custo = trepa_colinas_probabilistico(sol, grafo, vert, num_iter);
        // Escreve resultados da repeticao k
        printf("\nRepeticao %d:", k);
        escreve_sol(sol, vert);
        printf("\nCusto final: %2d\n", custo);
        mbf += custo;
        if (k == 0 || best_custo > custo) {
            best_custo = custo;
            substitui(best, sol, vert);
        }
    }
    // Escreve eresultados globais
    printf("\n\nMBF: %f\n", mbf / k);
    printf("\nMelhor solucao encontrada");
    escreve_sol(best, vert);
    printf("\nCusto final: %2d\n", best_custo);
    free(grafo);
    free(sol);
    free(best);
    return 0;
}


int algoritmo_evolutivo(int argc, char *argv[]) {
    char nome_fich[100];
    struct info EA_param;
    pchrom pop = NULL, parents = NULL;
    chrom best_run, best_ever;
    int gen_actual, r, runs, i, inv, mat[MAX_VERT][MAX_VERT];
    float mbf = 0.0;

    //Par�metros do problema
    EA_param.popsize = 100;
    EA_param.pm = 0.0;
    EA_param.pr = 0.7;
    EA_param.tsize = 2;
    EA_param.numGenerations = 2500;

    // L� os argumentos de entrada
    if (argc == 3) {
        runs = atoi(argv[2]);
        strcpy(nome_fich, argv[1]);
    } else
        // Se o n�mero de execu��es do processo n�o for colocado nos argumentos de entrada, define-o com um valor por defeito
    if (argc == 2) {
        runs = DEFAULT_RUNS;
        strcpy(nome_fich, argv[1]);
    }
        // Se o nome do ficheiro de informa��es n�o for colocado nos argumentos de entrada, pede-o novamente
    else {
        runs = DEFAULT_RUNS;
        printf("Nome do Ficheiro: ");
        fflush(stdin);
        gets(nome_fich);
    }
    // Se o n�mero de execu��es do processo for menor ou igual a 0, termina o programa
    if (runs <= 0)
        return 0;
    //Inicializa a gera��o dos n�meros aleat�rios
    init_rand();
    // Preenche a matriz com dados dos objectos (peso e valor) e a estrutura EA_param que foram definidos no ficheiro de input
    if(init_data(nome_fich, *mat, &EA_param) != 0)
        return 1;
    // Faz um ciclo com o n�mero de execu��es definidas
    for (r = 0; r < runs; r++) {
        printf("Repeticao %d\n", r + 1);
        // Gera��o da popula��o inicial
        pop = init_pop(EA_param);
        if(pop == NULL)
            return 1;
        // Avalia a popula��o inicial
        evaluate(pop, EA_param, *mat);
        gen_actual = 1;
        // Como ainda n�o existe, escolhe-se como melhor solu��o a primeira da popula��o (poderia ser outra qualquer)
        best_run = pop[0];
        // Encontra-se a melhor solu��o dentro de toda a popula��o
        best_run = get_best(pop, EA_param, best_run);
        // Reserva espa�o para os pais da popula��o seguinte
        parents = malloc(sizeof(chrom) * EA_param.popsize);
        // Caso n�o consiga fazer a aloca��o, envia aviso e termina o programa
        if (parents == NULL) {
            printf("Erro na alocacao de memoria\n");
            free(pop);
            return 1;
        }
        // Ciclo de optimiza��o
        while (gen_actual <= EA_param.numGenerations) {
            // Torneio bin�rio para encontrar os progenitores (ficam armazenados no vector parents)
            tournament(pop, EA_param, parents);
            //Selecao metodo roleta
            //roleta(pop, EA_param, parents);
            // Aplica os operadores gen�ticos aos pais (os descendentes ficam armazenados na estrutura pop)
            genetic_operators(parents, EA_param, pop);
            // Avalia a nova popula��o (a dos filhos)
            evaluate(pop, EA_param, *mat);
            // Actualiza a melhor solu��o encontrada
            best_run = get_best(pop, EA_param, best_run);
            gen_actual++;
        }

        // Contagem das solu��es inv�lidas
        for (inv = 0, i = 0; i < EA_param.popsize; i++)
            if (pop[i].valido == 0)
                inv++;
        // Escreve resultados da repeti��o que terminou
        printf("\nRepeticao %d:", r);
        write_best(best_run, EA_param);
        printf("\nPercentagem Invalidos: %f\n", 100 * (float) inv / EA_param.popsize);
        mbf += best_run.fitness;
        if (r == 0 || best_run.fitness < best_ever.fitness)
            best_ever = best_run;
        // Liberta a mem�ria usada
        free(parents);
        free(pop);
    }
    // Escreve resultados globais
    printf("\n\nMBF: %f\n", mbf / r);
    printf("\nMelhor solucao encontrada");
    write_best(best_ever, EA_param);
    return 0;
}

int algoritmo_hibrido(int argc, char *argv[]) {
    char nome_fich[100];
    struct info EA_param;
    pchrom pop = NULL, parents = NULL;
    chrom best_run, best_ever;
    int gen_actual, r, runs, i, inv, mat[MAX_VERT][MAX_VERT];
    float mbf = 0.0;

    //Par�metros do problema
    EA_param.popsize = 100;
    EA_param.numGenerations = 2500;
    EA_param.pm = 0.1;
    EA_param.pr = 0.3;
    EA_param.tsize = 2;

    // L� os argumentos de entrada
    if (argc == 3) {
        runs = atoi(argv[2]);
        strcpy(nome_fich, argv[1]);
    } else
        // Se o n�mero de execu��es do processo n�o for colocado nos argumentos de entrada, define-o com um valor por defeito
    if (argc == 2) {
        runs = DEFAULT_RUNS;
        strcpy(nome_fich, argv[1]);
    }
        // Se o nome do ficheiro de informa��es n�o for colocado nos argumentos de entrada, pede-o novamente
    else {
        runs = DEFAULT_RUNS;
        printf("Nome do Ficheiro: ");
        fflush(stdin);
        gets(nome_fich);
    }
    // Se o n�mero de execu��es do processo for menor ou igual a 0, termina o programa
    if (runs <= 0)
        return 0;
    //Inicializa a gera��o dos n�meros aleat�rios
    init_rand();
    // Preenche a matriz com dados dos objectos (peso e valor) e a estrutura EA_param que foram definidos no ficheiro de input
    if(init_data(nome_fich, *mat, &EA_param) != 0)
        return 1;
    // Faz um ciclo com o n�mero de execu��es definidas
    for (r = 0; r < runs; r++) {
        printf("Repeticao %d\n", r + 1);
        // Gera��o da popula��o inicial
        pop = init_pop(EA_param);
        if(pop == NULL)
            return 1;
        // Avalia a popula��o inicial
        evaluate(pop, EA_param, *mat);
        // Aplica��o do algoritmo trepa colinas para refinar a popula��o inicial
        trepa_colinas_hibrido(pop, EA_param, *mat);
        // Como ainda n�o existe, escolhe-se como melhor solu��o a primeira da popula��o (poderia ser outra qualquer)
        best_run = pop[0];
        // Encontra-se a melhor solu��o dentro de toda a popula��o
        best_run = get_best(pop, EA_param, best_run);
        // Reserva espa�o para os pais da popula��o seguinte
        parents = malloc(sizeof(chrom) * EA_param.popsize);
        // Caso n�o consiga fazer a aloca��o, envia aviso e termina o programa
        if (parents == NULL) {
            printf("Erro na alocacao de memoria\n");
            free(pop);
            return 1;
        }
        // Ciclo de optimiza��o
        gen_actual = 1;
        while (gen_actual <= EA_param.numGenerations) {
            // Torneio bin�rio para encontrar os progenitores (ficam armazenados no vector parents)
            tournament(pop, EA_param, parents);
            // Aplica os operadores gen�ticos aos pais (os descendentes ficam armazenados na estrutura pop)
            genetic_operators(parents, EA_param, pop);
            // Avalia a nova popula��o (a dos filhos)
            evaluate(pop, EA_param, *mat);
            // Actualiza a melhor solu��o encontrada
            best_run = get_best(pop, EA_param, best_run);
            gen_actual++;
        }

        //Aplicacao do algoritmo trepa colinas para refinar a populacao final
        //trepa_colinas_hibrido(pop, EA_param, *mat);

        // Contagem das solu��es inv�lidas
        for (inv = 0, i = 0; i < EA_param.popsize; i++)
            if (pop[i].valido == 0)
                inv++;
        // Escreve resultados da repeti��o que terminou
        printf("\nRepeticao %d:", r);
        write_best(best_run, EA_param);
        printf("\nPercentagem Invalidos: %f\n", 100 * (float) inv / EA_param.popsize);
        mbf += best_run.fitness;
        if (r == 0 || best_run.fitness < best_ever.fitness)
            best_ever = best_run;
        // Liberta a mem�ria usada
        free(parents);
        free(pop);
    }
    // Escreve resultados globais
    printf("\n\nMBF: %f\n", mbf / r);
    printf("\nMelhor solucao encontrada");
    write_best(best_ever, EA_param);
    return 0;
}

int main(int argc, char *argv[]) {
    int op;
    do{
        printf("\nAlgoritmo\n");
        printf("1 - Trepa-Colinas\n");
        printf("2 - Evolutivo\n");
        printf("3 - Hibrido\n");
        printf("4 - Sair\n");
        printf("Selecione uma opcao: ");
        scanf("%d", &op);
        switch (op) {
            case 1:
                if(algoritmo_trepa_colinas(argc, argv) != 0){
                    printf("\nAlgo Correu mal durante a execucao. Tente outra vez.\n");
                }
                break;
            case 2:
                if(algoritmo_evolutivo(argc, argv) != 0){
                    printf("\nAlgo Correu mal durante a execucao. Tente outra vez.\n");
                }
                break;
            case 3:
                if(algoritmo_hibrido(argc, argv) != 0){
                    printf("\nAlgo Correu mal durante a execucao. Tente outra vez.\n");
                }
                break;
            case 4:
                printf("A fechar.");
                break;
            default:
                printf("N�o implementado\n");
                break;
        }
    }while(op != 4);

    return 0;
}

