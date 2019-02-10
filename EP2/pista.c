/* ******************************************************************* */
/*                                                                     */
/*                 MAC0422 - Sistemas Operacionais                     */
/*                       Exercício-programa 2                          */
/*                                                                     */
/*              Bárbara de Castro Fernandes - 7577351                  */
/*            Taís Aparecida Pereira Pinheiro - 7580421                */
/*                                                                     */
/*                       Arquivo: pista.c                              */
/* ******************************************************************* */

#include "pista.h"
#include "aux.h"
#include "ciclista.h"
#include <unistd.h>

/* Cria a pista da corrida*/
void criaPista (int d)
{
    int i, j;

    pista = mallocSafe (d * (sizeof (int*)));
    for(i = 0; i < d; i++)
        pista[i] = mallocSafe (10 * (sizeof (int)));

    for (i = 0; i < d; i++)
    {
        for (j = 0; j < 10; j++)
        {
            pista[i][j] = -1;
        }
    }
    dc = d;
}

/* Habilita pista para corrida com semaforos*/
void habilitaPista (int d)
{
    int i, j;

    local = mallocSafe (d * (sizeof (sem_t*)));
    for(i = 0; i < d; i++)
        local[i] = mallocSafe (10 * (sizeof (sem_t)));

    for (i = 0; i < d; i++)
    {
        for (j = 0; j < 10; j++)
            if(sem_init(&local[i][j], 0, 1))
            {
                printf("Erro ao criar o semáforo da pista!\n");
                exit(EXIT_FAILURE);
            }
    }

    /* Posiciona os primeiros colocados*/
    if(pthread_barrier_init(&barreiraPista, NULL, 10))
    {
        printf("\nErro na criação da Barreira da Pista!\n");
        exit(EXIT_FAILURE);
    }
}

/* Mostra a pista com os devidos ciclistas*/
void mostraPista (int d)
{
    int i, j;

    for (i = d-1; i >= 0; i--)
    {
        for (j = 0; j < 10; j++)
        {
            if (pista[i][j] == -1)
                printf ("| X  | ");
            else
                printf("| %d | ",pista[i][j]);
        }
        printf ("\n");
    }
}

/* Deleta a pista*/
void deletaPista(int d)
{
    int i;

    for(i = 0; i < d; i++)
        free(pista[i]);
    free (pista);
}

/* Deleta os semaforos da pista*/
void deletaLocal(int d)
{
    int i;

    for(i = 0; i < d; i++)
        free(local[i]);
    free (local);
}
