/* ***************************************************** */
/*                                                       */
/*        EP1 de MAC0422 - Sistemas Operacionais         */
/*                                                       */
/*         B'arbara de Castro Fernandes -7577351          */
/*         Mauricio Luiz Abreu Cardoso - 6796479         */
/*                                                       */
/* ***************************************************** */

#include <stdio.h>
#include <string.h>
#include "leitura.h"

/* Le a partir da entrada padrao e aloca em pal o resultado
// ao receber um caractere em branco, sendo que o caractere
// em branco 'e trocado por "\0". Devolve 0 se o ultimo caractere
// lido for "\n", -1 se o ultimo caractere for 'EOF', e 1 em
// todos os outros casos */

static int leStringTeclado (char *pal) {
    int c, i = 0;

    do {
        c = getc (stdin);
        pal[i] = c;
        i++;
    } while (!((c >= 0 && c <= 32) || c == EOF));

    pal[i-1] = '\0';

    if (c == '\n') return 0;
    else if (c == EOF) return -1;
    else return 1;
}

int alocaPalavras (char **argumentosUsuario) {
    int conta = 0, c;
    while ((c = leStringTeclado (argumentosUsuario[conta])) == 1)
        if (argumentosUsuario[conta] != '\0') /* Testa se leu uma palavra */
            conta++;
    if (c == 0 && argumentosUsuario[0][0] == '\0')
        return alocaPalavras (argumentosUsuario);
    else if (c == -1)
        return -1;
    else
        return conta + 1;
}
