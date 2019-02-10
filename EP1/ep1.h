/* ******************************************************************* */
/*                                                                     */
/*                 MAC0422 - Sistemas Operacionais                     */
/*                       Exercício-programa 1                          */
/*                                                                     */
/*              Bárbara de Castro Fernandes - 7577351                  */
/*            Taís Aparecida Pereira Pinheiro - 7580421                */
/*                                                                     */
/* ******************************************************************* */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <linux/limits.h> /* Contém várias macros relevantes */
#include <semaphore.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#define QUANTUM 1

typedef struct processo
{
    pthread_t *thread;      /* Thread que representa esse processo */
    int id;                 /* id do processo */
    char nome[NAME_MAX];    /* Nome identificador do processo */
    float t0;               /* Tempo em que o processo chega */
    float dt;               /* Tempo real de processamento */
    float deadline;         /* Tempo limite para finalizacao */
    int p;                  /* Prioridade */
    int linha;              /* Linha no arquivo de trace */
    float tr;               /* Tempo de relógio */
    float tf;               /* Tempo de término */
    struct processo *prox;  /* ponteiro para o proxim processo */
    struct processo *ant;
    int ativo;
    int chegada;

} Processo;
