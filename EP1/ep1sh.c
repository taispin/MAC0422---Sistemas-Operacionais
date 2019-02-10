/* ******************************************************************* */
/*                                                                     */
/*                 MAC0422 - Sistemas Operacionais                     */
/*                       Exercício-programa 1                          */
/*                                                                     */
/*              Bárbara de Castro Fernandes - 7577351                  */
/*            Taís Aparecida Pereira Pinheiro - 7580421                */
/*                                                                     */
/* ******************************************************************* */

/* *** Bibliotecas inclusas *** */

#include "ep1sh.h"

/* *** Macros criadas *** */

#define SHORTEST_JOB_FIRST 1
#define ROUND_ROBIN 2
#define PRIORITY_SCALING 3

/* *** Protótipo das funções *** */

void date ();
char *typePrompt ();
void readCommand();
void *mallocSafe (size_t size);
void printErrno();
void limpaString (char *string);
void processLine (char *line);
void callChown (char *line);
void callExecve (char *command,char **argv,char**env);
void callPing (char *line, char *command);
void callCal (char *line, char *command);
void callEp1 (char *line, char *command);

/* *** Começo da implementação das funções *** */

int main(int argc, char *argv[])
{
    while(1)
    {
        /* Imprime o diretório atual, recebe e interpreta o comando digitado. */
        readCommand();
    }

    return EXIT_SUCCESS;
}

/* Imprime o prompt do shell, que contém o diretório atual entre colchetes
 * seguido de "$" e de um espaço em branco. */
char *typePrompt ()
{
    char *prompt, *directory;
    prompt = mallocSafe (PATH_MAX * sizeof (char));
    directory = mallocSafe (PATH_MAX * sizeof (char));
    directory = getcwd (directory, PATH_MAX);

    if (directory == NULL)
    {
        printf ("Erro! Código do erro: %d\n", errno);
        printErrno ();
    }

    sprintf (prompt, "[%s]$ ", directory);
    return prompt;
}


/* Lê a linha que foi digitada e verifica se ela é um comando para sair
 * do prompt, se é uma linha vazia ou algum outro tipo de comando. */
void readCommand()
{
    char *prompt = typePrompt(), *line = readline (prompt);

    if (line == NULL || strcmp (line, "exit") == 0 || strcmp (line, "quit") == 0)
    {
        printf ("\nFim de transmissão\n");
        exit (EXIT_FAILURE);
    }

    if (*line)
    {
        add_history (line);
    }
    else
        printf ("Uma linha foi digitada, mas ela é vazia.\n");

    processLine (line);
    free (line);
}

/* Recebe a linha digitada pelo usuário e realiza a sua interpretação,
 * identificando qual comando foi digitado e quais são os seus argumentos. */
void processLine (char *line)
{
    char *command = strtok (line, " ");

    /* Comando "date": não recebe nenhum argumento e devolve a data
     * e hora atual. */
    if (strcmp (line, "date") == 0)
        date();

    /* Comando "chown": recebe como argumentos o nome do grupo ao qual
     * queremos que o arquivo pertença - antecedido por ":" - e o nome
     * do arquivo que queremos mudar o grupo. */
    else if (strcmp (command, "chown") == 0)
        callChown (line);

    /* Comando "ping": envia pacotes através do protocolo ICMP. Quando
     * o destinatário recebe um pacote, envia uma resposta. Quando essa
     * resposta retorna, o tempo decorrido é contado em milissegundos.
     * Recebe como parâmetros uma flag, a quantidade de pacotes que desejamos
     * enviar ao destinatário e o endereço do destinatário. */
    else if (strcmp (command, "/bin/ping") == 0)
        callPing (line, command);

    /* Comando "cal": recebe como argumento algum ano e imprime o
     * calendário deste ano. */
    else if (strcmp (command, "/usr/bin/cal") == 0)
        callCal (line, command);

    /* Comando "ep1": chama o simulador de processos. Recebe como argumentos
     * o número identificador de qual algoritmo de escalonamento deve ser
     * utilizado, o nome de um arquivo de trace e o nome do arquivo que
     * deverá será criado pelo simulador. */
    else if (strcmp (command, "./ep1") == 0)
        callEp1 (line, command);

    /* Caso o comando recebido não seja nenhuma das opções anteriores,
     * será considerado um comando inválido. */
    else
        printf ("Comando \"%s\" não identificado.\n", command);
}

void callPing (char *line, char *command)
{
    pid_t pid;

    if ((pid = fork()) == 0)
    {
        int s;
        char *argv[5], *env[] = { NULL };

        argv[0] = command;
        argv[4] = NULL;
        for (s = 1; s < 4; s++) argv[s] = strtok (NULL, " ");

        callExecve (command, argv, env);
    }
    else
    {
        int ret;
        ret = waitpid (pid, NULL, 0);
        if (ret != pid)
        {
            printf ("Erro: ret != waitpid\n");
            printErrno ();
            exit (EXIT_FAILURE);
        }
    }
}

/* Comando "chown". Uso: chown :<grupo> <arquivo no diretório atual>. */
void callChown (char *line)
{
    int chownReturn;
    gid_t groupNumber;
    char *group = strtok (NULL, " :"), *filename = strtok (NULL, "");
    struct group *teste = getgrnam (group);


    if (group == NULL || filename == NULL)
    {
        printf ("Erro! Argumentos da função chown inválidos\n");
        exit (EXIT_FAILURE);
    }

    if (teste == NULL)
    {
        printf ("Grupo não encontrado.\n");
        exit (EXIT_FAILURE);
    }

    groupNumber = teste->gr_gid;
    printf ("group: %s, groupNumber: %d\n", group, groupNumber);
    chownReturn = chown (filename, (uid_t) -1, groupNumber);
    if (chownReturn)
    {
        printf ("Erro! Código do erro: %d\n", errno);
        printErrno ();
        exit (EXIT_FAILURE);
    }
    else
    {
        printf ("Comando chown executado com sucesso.\n");
    }
}

void callExecve (char *command, char **argv, char **env)
{
    int s = execve (command, argv, env);

    if (s == -1)
    {
        printf ("Erro: s == -1\n");
        printErrno ();
        exit (EXIT_FAILURE);
    }
    exit (s);
}

void callCal (char *line, char *command)
{
    pid_t pid;

    if ((pid = fork()) == 0)
    {
        char *argv[3], *env[] = {NULL};
        argv[0] = command;
        argv[1] = strtok (NULL, " ");
        argv[2] = NULL;

        callExecve (command, argv, env);
    }
    else
    {
        int ret;
        ret = waitpid (pid, NULL, 0);
        if (ret != pid)
        {
            printf ("Erro: ret != pid\n");
            printErrno ();
            exit (EXIT_FAILURE);
        }
    }
}

void callEp1 (char *line, char *command)
{
    pid_t pid;

    if ((pid = fork()) == 0)
    {
        char *argv[6], *env[] = {NULL};
        argv[0] = command;
        argv[1] = strtok (NULL, " ");
        argv[2] = strtok (NULL, " ");
        argv[3] = strtok (NULL, " ");
        argv[4] = strtok (NULL, " ");
        argv[5] = NULL;

        if (argv[0] == NULL || argv[1] == NULL || argv[2] == NULL)
        {
            printf ("Erro! Argumentos da função ./ep1 inválidos\n");
            exit (EXIT_FAILURE);
        }

        callExecve (command, argv, env);
    }
    else
    {
        int ret = waitpid (pid, NULL, 0);
        if (ret != pid)
        {
            printf ("Erro: ret != pid\n");
            printErrno ();
            exit (EXIT_FAILURE);
        }
    }
}

/*

   struct group *getgrnam(const char *name);
   int getgrnam_r(const char *name, struct group *grp, char *buffer,
    size_t bufsize, struct group **result);

 */

/* Comando "date". Uso: date. */
void date()
{
    int i;
    time_t tempo;
    struct tm * info;
    char* part = mallocSafe (10 * sizeof (char));
    char* formatDate = mallocSafe (30 * sizeof (char));

    limpaString (part);
    limpaString (formatDate);

    time (&tempo);
    info = localtime (&tempo);

    /* Informacoes de data e hora*/
    sprintf (formatDate, "%s", asctime (info));

    /* Informacoes de Timezone e ano*/
    strftime (part, 10, "%Z %Y", info);

    for (i = 0; i < strlen (part); i++)
        formatDate[i+20] = part[i];

    formatDate[29] = '\0';

    printf ("%s\n", formatDate);

    free (part);
    free (formatDate);
}


/***************** FUNÇÕES DE SUPORTE ********************/

void *mallocSafe (size_t size)
{
    void *ptr = malloc (size);

    if (ptr == NULL)
    {
        fprintf (stderr, "Erro durante alocação dinâmica de memória!\n");
        exit (EXIT_FAILURE);
    }

    return ptr;
}


void printErrno ()
{
    printf ("EINVAL: %d\nERANGE: %d\nEACCES: %d\nENOMEM: %d\nEPERM: %d\n",
            EINVAL, ERANGE, EACCES, ENOMEM, EPERM);
}

void limpaString(char *string)
{
    int i = 0,t = strlen(string);
    for(i = 0; i < t; i++) string[i] = ' ';
}
