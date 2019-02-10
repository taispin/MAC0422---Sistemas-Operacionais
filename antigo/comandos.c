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
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "comandos.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2

void protegepracaramba (char *argumentoUsuario) {
    mode_t protecao = 0000;

    if (chmod (argumentoUsuario, protecao) != 0) {
        printf ("Erro na mudanca de permissoes do arquivo!\n");
        exit (EXIT_FAILURE);
    }
}

void liberageral (char *argumentoUsuario) {
    mode_t protecao = 0777;

    if (chmod (argumentoUsuario, protecao) != 0) {
        printf ("Erro na mudanca de permissoes do arquivo!\n");
    }
}

void rodeveja (char *argumentosUsuario[]) {
    pid_t pid;
    int codigo;
    char *newEnviron[] = { NULL };

    if ((pid = fork ()) == 0)
        execve (argumentosUsuario[0], argumentosUsuario, newEnviron);
    else if (pid == -1) {
        printf ("Erro na criacao de um novo processo!\n");
        exit (EXIT_FAILURE);
    }
    else {
        wait (&codigo);
        printf ("Codigo de retorno: %d\n", codigo);
    }
}

void rode (char *argumentosUsuario[]) {
    pid_t pid;
    char *newEnviron[] = { NULL };

    if ((pid = fork ()) == 0) {
        close (STDIN);
        close (STDOUT);
        close (STDERR);
        execve (argumentosUsuario[0], argumentosUsuario, newEnviron);
    }
    else if (pid == -1) {
        printf ("Erro na criacao de um novo processo!\n");
        exit (EXIT_FAILURE);
    }
}
