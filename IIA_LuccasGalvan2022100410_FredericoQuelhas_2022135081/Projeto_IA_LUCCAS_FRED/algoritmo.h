
#ifndef PROJETO2IIA_ALGORITMO_H
#define PROJETO2IIA_ALGORITMO_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

void gera_vizinho(int a[], int b[], int n);

void gera_vizinho2(int a[], int b[], int n);

int trepa_colinas(int sol[], int *mat, int vert, int num_iter);

int trepa_colinas_probabilistico(int sol[], int *mat, int vert, int num_iter);

#endif //PROJETO2IIA_ALGORITMO_H

