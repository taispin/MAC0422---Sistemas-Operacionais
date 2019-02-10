/* ******************************************************************* */
/*                                                                     */
/*                 MAC0422 - Sistemas Operacionais                     */
/*                       Exercício-programa 2                          */
/*                                                                     */
/*              Bárbara de Castro Fernandes - 7577351                  */
/*            Taís Aparecida Pereira Pinheiro - 7580421                */
/*                                                                     */
/*                         Arquivo: aux.h                              */
/* ******************************************************************* */

#ifndef _AUX_H_
#define _AUX_H_
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "ciclista.h"

void *mallocSafe (size_t size);
void relatorio(int n, int v, int modo, int q);
void organizaPorPontos(Ciclista *x);

#endif
