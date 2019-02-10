/* ******************************************************************* */
/*                                                                     */
/*                 MAC0422 - Sistemas Operacionais                     */
/*                       Exercício-programa 2                          */
/*                                                                     */
/*              Bárbara de Castro Fernandes - 7577351                  */
/*            Taís Aparecida Pereira Pinheiro - 7580421                */
/*                                                                     */
/*                         Arquivo: aux.c                              */
/* ******************************************************************* */

#include "aux.h"


void *mallocSafe (size_t size)
{
    void *ptr = malloc (size);

    if (ptr == NULL)
    {
        fprintf (stderr, "Erro durante alocação dinâmica de memória!\n");
        exit (EXIT_FAILURE);
    }

    return ptr;
}

void relatorio(int n, int v, int modo, int q)
{
    int i,j;
    Ciclista *x;

    x = mallocSafe(n*sizeof(Ciclista));

    if(modo == 1)
    {
        printf("\n\n ------> RELATORIO DA VOLTA [ %d ] <--------\n\n",v+1);
        for(i = 0; i < n; i++)
            if(ciclistas[i].ativo == 1)
                printf("%dº Lugar --> Ciclista com id:%d\n",i+1,ciclistas[i].id);

        if(q == 1)
        {
            printf("\n Ciclistas Quebrados \n");
            for(i = 0; i < n; i++)
                if(ciclistas[i].ativo == 0 || ciclistas[i].ativo == -1 )
                {
                    printf("-> Ciclista com id:%d\n",ciclistas[i].id);
                    j--;
                }
        }
    }

    if(modo == 2)
    {
        printf("\n\n ------> RELATORIO DA VOLTA [ %d ] COM PONTUACAO <--------\n\n",v+1);

        organizaPorPontos(x);

        for(j = n, i = n-1; i >= 0; i--)
            if(x[i].ativo == 1)
            {
                printf("%dº Lugar --> Ciclista com id:%d com Pontuacao:%d\n",x[i].colocacao,x[i].id,x[i].pontuacao);
                j--;
            }
        if(q == 1)
        {
            printf("\n Ciclistas Quebrados \n");
            for(i = n-1; i >= 0; i--)
                if(x[i].ativo == 0 || x[i].ativo == -1 )
                {
                    printf("-> Ciclista com id:%d com Pontuacao:%d\n",x[i].id,x[i].pontuacao);
                    j--;
                }
        }
    }

    if(modo == 3)
    {
        printf("\n\n ------> RELATORIO FINAL <--------\n\n");

        organizaPorPontos(x);

        printf("-------> CLASSIFICACAO POR PONTOS <--------\n\n");
        for(j = n, i = n-1; i >= 0; i--, j--)
            if(x[i].ativo == 1)
                printf("%dº Lugar --> Ciclista com id:%d com Pontuacao:%d.\n Tempo de chagada:%f\n\n",j,x[i].id,x[i].pontuacao,x[i].chegada);
            else
                printf("[CICLISTA QUEBRADO]--> Ciclista com id:%d com Pontuacao:%d.\n Quebrou na volta:%d\n\n",x[i].id,x[i].pontuacao,x[i].volta);

        if(q == 1)
        {
            printf("\n Ciclistas Quebrados \n");
            for(i = n-1; i >= 0; i--)
                if(x[i].ativo == 0 || x[i].ativo == -1 )
                {
                    printf("-> Ciclista com id:%d com Pontuacao:%d\n",x[i].id,x[i].pontuacao);
                    j--;
                }
        }
    }
    printf("\n\n");
    free(x);
}


void organizaPorPontos(Ciclista *x)
{
    int i,j,k;
    Ciclista aux;

    /* Faz a copia para o vetor x*/
    for (j = 0; j < n; j++)
    {
        x[j].ativo = ciclistas[j].ativo;
        x[j].id = ciclistas[j].id;
        x[j].livre = ciclistas[j].livre;
        x[j].pontuacao = ciclistas[j].pontuacao;
        x[j].tempoVoltaAtual = ciclistas[j].tempoVoltaAtual;
        x[j].v = ciclistas[j].v;
        x[j].vAntes = ciclistas[j].vAntes;
        x[j].volta = ciclistas[j].volta;
        x[j].colocacao = ciclistas[j].colocacao;
        x[j].chegada = ciclistas[j].chegada;
    }

    /* Ordena */

    for (j = 1; j < n; ++j)
    {
        k = x[j].pontuacao;
        aux.ativo = x[j].ativo;
        aux.id = x[j].id;
        aux.livre = x[j].livre;
        aux.pontuacao = x[j].pontuacao;
        aux.tempoVoltaAtual = x[j].tempoVoltaAtual;
        aux.v = x[j].v;
        aux.vAntes = x[j].vAntes;
        aux.volta = x[j].volta;
        aux.colocacao = x[j].colocacao;
        aux.chegada = x[j].chegada;

        for (i = j-1; i >= 0 && x[i].pontuacao > k; --i)
        {
            x[i+1].ativo = x[i].ativo;
            x[i+1].id = x[i].id;
            x[i+1].livre = x[i].livre;
            x[i+1].pontuacao = x[i].pontuacao;
            x[i+1].tempoVoltaAtual = x[i].tempoVoltaAtual;
            x[i+1].v = x[i].v;
            x[i+1].vAntes = x[i].vAntes;
            x[i+1].volta = x[i].volta;
            x[i+1].colocacao = x[i].colocacao;
            x[i+1].chegada = x[i].chegada;
        }

        x[i+1].ativo = aux.ativo;
        x[i+1].id = aux.id;
        x[i+1].livre = aux.livre;
        x[i+1].pontuacao = aux.pontuacao;
        x[i+1].tempoVoltaAtual = aux.tempoVoltaAtual;
        x[i+1].v = aux.v;
        x[i+1].vAntes = aux.vAntes;
        x[i+1].volta = aux.volta;
        x[i+1].colocacao = aux.colocacao;
        x[i+1].chegada = aux.chegada;
    }
}
