

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef PROJETO2IIA_UTILS_H
#define PROJETO2IIA_UTILS_H

int* init_dados(char *nome, int *n, int *tam);

void gera_sol_inicial(int *sol, int v, int tam);

void escreve_sol(int *sol, int vert);

void substitui(int a[], int b[], int n);

void init_rand();

int random_l_h(int min, int max);

float rand_01();

#endif //PROJETO2IIA_UTILS_H
