/* ***************************************************** */
/*                                                       */
/*        EP1 de MAC0422 - Sistemas Operacionais         */
/*                                                       */
/*         B'arbara de Castro Fernandes -7577351          */
/*         Mauricio Luiz Abreu Cardoso - 6796479         */
/*                                                       */
/* ***************************************************** */

#ifndef __COMANDOS_H__
#define __COMANDOS_H__

/* Recebe o caminho de um arquivo e muda a sua permissao para que
// nenhum usuario possa le-lo, executa-lo e abri-lo */

void protegepracaramba (char *argumentoUsuario);

/* Recebe o caminho relativo de um arquivo e muda a sua permissao
// para que todos os usuarios possam le-lo, executa-lo e abri-lo */

void liberageral (char *argumentoUsuario);

/* Recebe um vetor de strings com nome, tal que argumentosUsuario[0]
// contem o caminho do programa a ser executado, os elementos seguintes
// contem as flags, e termina necessariamente com NULL. Executa o
// programa em foreground */

void rodeveja (char *argumentosUsuario[]);

/* Recebe um vetor de strings com nome, tal que argumentosUsuario[0]
// contem o caminho do programa a ser executado, os elementos seguintes
// contem as flags, e termina necessariamente com NULL. Executa o
// programa em background */

void rode (char *argumentosUsuario[]);

#endif
