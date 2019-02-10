/* ******************************************************************* */
/*                                                                     */
/*                 MAC0422 - Sistemas Operacionais                     */
/*                       Exercício-programa 2                          */
/*                                                                     */
/*              Bárbara de Castro Fernandes - 7577351                  */
/*            Taís Aparecida Pereira Pinheiro - 7580421                */
/*                                                                     */
/*                       Arquivo: pista.h                              */
/* ******************************************************************* */

#ifndef _PISTA_H_
#define _PISTA_H_
#include <semaphore.h>
#include <pthread.h>


int **pista,dc;
sem_t **local, tela;
pthread_barrier_t barreiraPista; /* barreira de sincronizacao*/

void criaPista (int d);
void habilitaPista (int d);
void mostraPista (int d);
void deletaPista(int d);
void deletaLocal(int d);


#endif
