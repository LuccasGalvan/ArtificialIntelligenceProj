
#include <stdbool.h>
#include "funcao.h"

// Calcula a qualidade de uma solu��o
// Recebe:  A solu��o, a, a matriz de adjac�ncias, mat, e o n�mero de v�rtices, vert
// Devolve: O custo que � o n�mero de liga��es que existem na solu��o
int calcula_fit(int a[], int *mat, int vert)
{
    int total=0;
    int i, j;
    bool temConexao;

    for(i=0; i<vert; i++)
        if(a[i]==1)
        {
            //printf("No: %d\t", i);
            temConexao = false;
            for(j=0; j<vert;j++){
                if(a[j]==1 && *(mat + (vert) * (j) + i) != 9999){
                    temConexao = true;
                    //printf("Custo: %d\n", *(mat + (vert) * (j) + i));
                    total += *(mat + (vert) * (j) + i);//*(mat+vert*(i+j));//getCustoVertice(a, mat, vert, j);
                }
            }
            if(!temConexao){
                total += 9999;
                break;
            }
                //if(&& *(mat+i*vert+j)==1)
                    //total++;
        }
    return total/2;
}

