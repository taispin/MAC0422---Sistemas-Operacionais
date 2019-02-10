/* ******************************************************************* */
/*                                                                     */
/*                 MAC0422 - Sistemas Operacionais                     */
/*                       Exercício-programa 1                          */
/*                                                                     */
/*              Bárbara de Castro Fernandes - 7577351                  */
/*            Taís Aparecida Pereira Pinheiro - 7580421                */
/*                                                                     */
/* ******************************************************************* */

#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* strcmp: usando para comparar duas palavras*/
#include <unistd.h>
#include <assert.h>
#include <sys/syscall.h> /* chamadas de sistema*/
#include <readline/readline.h> /* ler a linha digitada do shell*/
#include <readline/history.h>
#include <errno.h> /* Número do último erro ocorrido */
#include <linux/limits.h> /* Contém várias macros relevantes */
#include <time.h>
#include <grp.h> /* Auxilia na obtenção do id do grupo a partir de seu nome */
#include <pthread.h>
#include <sys/wait.h>
