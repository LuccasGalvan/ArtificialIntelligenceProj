
#ifndef PROJETO2IIA_FUNCAO_EVOLUTIVO_H
#define PROJETO2IIA_FUNCAO_EVOLUTIVO_H

#include "funcao.h"

float eval_individual_reparado1(int sol[], struct info d, int * mat, int *v);

void repara_solucao(int sol[], struct info d, int *mat, int *v);

void evaluate(pchrom pop, struct info d, int *mat);

void gera_vizinho_hibrido(int sol[], int solViz[], int *mat, int nGenes);

void trepa_colinas_hibrido(pchrom pop, struct info d, int *mat);

#endif //PROJETO2IIA_FUNCAO_EVOLUTIVO_H

