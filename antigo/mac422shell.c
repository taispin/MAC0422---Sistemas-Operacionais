/* ***************************************************** */
/*                                                       */
/*        EP1 de MAC0422 - Sistemas Operacionais         */
/*                                                       */
/*         B'arbara de Castro Fernandes -7577351          */
/*         Mauricio Luiz Abreu Cardoso - 6796479         */
/*                                                       */
/* ***************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comandos.h"
#include "leitura.h"

#define PROTEGEPRACARAMBA 0
#define LIBERAGERAL 1
#define RODEVEJA 2
#define RODE 3
#define EXIT 4
#define TAM_MAX_PALAVRA 64
#define TRUE 1
#define FALSE 0
#define MAX_ARGUMENTOS 30

/* Recebe um vetor de strings v com n strings. Apos sua execucao,
// v[0] = v[1], v[1] = v[n-1] e v[n-1] = v[0] */

void trocaIndices (char **v, int n) {
    char *aux = v[0];
    int i;
    for (i = 0; i < n - 1; i++) {
        v[i] = v[i+1];
    }
    v[n-1] = aux;
}

int main (int argc, char *argv[]) {
    const char *comandos[] = {"protegepracaramba", "liberageral",
     "rodeveja", "rode", "exit"};
     char *aux, **argumentosUsuario = malloc (MAX_ARGUMENTOS * sizeof (char *));
     int i, n;

    for (i = 0; i < MAX_ARGUMENTOS; i++)
        argumentosUsuario[i] = malloc (TAM_MAX_PALAVRA * sizeof (char));

    while (TRUE) {
        /* n 'e o total de strings validas alocadas em argumentosUsuario */
        n = alocaPalavras (argumentosUsuario);
        if (n > 0) /* Leu palavras */ {
            if (strcmp (argumentosUsuario[0], comandos[PROTEGEPRACARAMBA]) == 0) {
                protegepracaramba (argumentosUsuario[1]);
            }
            else if (strcmp (argumentosUsuario[0], comandos[LIBERAGERAL]) == 0) {
                liberageral (argumentosUsuario[1]);
            }
            else if (strcmp (argumentosUsuario[0], comandos[RODEVEJA]) == 0) {
                trocaIndices (argumentosUsuario, n);
                /* Agora argumentosUsuario[0] possui o comando e
                argumentosUsuario[1...n-2] as flags */
                aux = argumentosUsuario[n-1];
                /* Necessario para a chamada de execve */
                argumentosUsuario[n-1] = NULL;
                rodeveja (argumentosUsuario);
                argumentosUsuario[n-1] = aux;
            }
            else if (strcmp (argumentosUsuario[0], comandos[RODE]) == 0) {
                trocaIndices (argumentosUsuario, n);
                /* Agora argumentosUsuario[0] possui o comando e
                argumentosUsuario[1...n-2] as flags */
                aux = argumentosUsuario[n-1];
                /* Necessario para a chamada de execve */
                argumentosUsuario[n-1] = NULL;
                rode (argumentosUsuario);
                argumentosUsuario[n-1] = aux;
            }
            else if (strcmp (argumentosUsuario[0], comandos[EXIT]) == 0)
                break;
            else
                printf ("Comando nao reconhecido!\n");
        }
        else
            break;
    }

    for (i = 0; i < MAX_ARGUMENTOS; i++)
        free (argumentosUsuario[i]);
    free (argumentosUsuario);
    return EXIT_SUCCESS;
}
