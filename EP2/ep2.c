/* ******************************************************************* */
/*                                                                     */
/*                 MAC0422 - Sistemas Operacionais                     */
/*                       Exercício-programa 2                          */
/*                                                                     */
/*              Bárbara de Castro Fernandes - 7577351                  */
/*            Taís Aparecida Pereira Pinheiro - 7580421                */
/*                                                                     */
/*                    Arquivo: ep2-barbara-tais.c                      */
/* ******************************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ciclista.h"
#include "aux.h"
#include "pista.h"

int n,d,v;

int main (int argc, char *argv[])
{
    /* 'd' representa o tamanho da pista, 'n' o número de ciclistas e 'v' o
     * número de voltas. Também é possível se passar uma opção a mais para debug */
    int i,k, s = 0;
    pthread_t simular;

    if (argc != 4 && argc != 5)
    {
        printf ("Número inválido de argumentos!\n");
        return EXIT_FAILURE;
    }

    d = atoi (argv[1]);
    n = atoi (argv[2]);
    v = atoi (argv[3]);
    muda = 0;

    printf("d:%d, n:%d, v:%d\n",d,n,v);

    iniciaD(d);
    criaPista (d);
    habilitaPista(d);
    mostraPista(d);

    inicializaCiclistas(n);
    iniciaControladores();
    mostraCiclistas();

    /* Opcao DEBUG*/
    if(argc == 5)
        pthread_create(&simular, NULL, show,NULL);

    /* Tempo de largada*/
    time(&start);

    /* Gerencia as voltas */
    for(i = 0; i < v; i++)
    {
        if(i == v-2) escolheCiclistaAleatorio();

        /* O vetor deve estar ordenado por posicao = colocacao*/
        for(k = 0; k < nCiclistasAtivos; k++)
        {
            if(ciclistas[k].ativo == 1)
                pthread_create(&ciclistas[k].thread, NULL, volta,(void *) &ciclistas[k].colocacao);
        }
        /* Espera todo mundo terminar*/
        while(s != 1)
            s = seguir();
        segue = 0;
        s =  0;

        /*Arruma a casa pra proxima volta*/
        organizaCiclistas();

        /* Atualiza as pontuacoes*/
        if(i%10 == 9) atualizaPontuacao();

        /* Mostra o relatorio maior*/
        if(i%10 == 9)
        {
            if(n != nCiclistasAtivos)
            {
                sem_wait (&tela);
                relatorio(n,i,1,1);
                relatorio(n,i,2,1);
                sem_post (&tela);
            }
            else
            {
                sem_wait (&tela);
                relatorio(n,i,1,0);
                relatorio(n,i,2,0);
                sem_post (&tela);
            }
            usleep(0.20*1000000);
        }
        /* Mostra o relatorio menor*/
        else
        {
            if(n != nCiclistasAtivos)
            {
                sem_wait (&tela);
                relatorio(n,i,1,1);
                sem_post (&tela);
            }
            else
            {
                sem_wait (&tela);
                relatorio(n,i,1,0);
                sem_post (&tela);
            }

            usleep(0.20*1000000);
        }
    }

    /* Ajusta o tempo de chegada*/
    tempoDeChegada();
    muda = 2;

    /* Imprime relatorio final*/
    if(n != nCiclistasAtivos)
    {
        sem_wait (&tela);
        relatorio(n,v,3,1);
        sem_post (&tela);
    }

    else
    {
        sem_wait (&tela);
        relatorio(n,v,3,0);
        sem_post (&tela);
    }


    deletaPista(d);
    deletaLocal(d);

    return EXIT_SUCCESS;
}

