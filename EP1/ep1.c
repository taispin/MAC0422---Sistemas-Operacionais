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

#include "ep1.h"

/* *** Protótipo das funções *** */

int calculaNucleos ();
void *mallocSafe (size_t size);
Processo *criaFilaProcessos (char* fileName);
void verFila (Processo *fila);
void ordenarSubLista(Processo *inicio, Processo *fim);
void ordenaFilaSJF(Processo *fila);
void *processoNaoPreemptivo(void *param);
void ocupaCPU(float tempo);
void saiDaFila(Processo *p);
void *processoPreemptivo (void *param);
void roundRobin (char *outFile);
void shortestJobFirst (char *outFile);
void escalonamentoComPrioridade (char *outFile);
void ordenaFilaECP (Processo *fila);

int numProc = 0;
int numMudContexto = 0;
sem_t semNucleos;
sem_t semAlteraFila;
sem_t semStd;
Processo *filaProcesso;
int std = 0;
int l = 1;
time_t start;

/* *** Funçao principal *** */

int main (int argc, char *argv[])
{
    int opt = atoi (argv[1]), numNucleos = calculaNucleos();
    char *fileName = mallocSafe (NAME_MAX * sizeof (char));
    char *fileOut = mallocSafe (NAME_MAX * sizeof (char));


    if (argc != 4 && argc != 5)
    {
        printf ("Número inválido de argumentos.\n");
        return EXIT_FAILURE;
    }

    if(argc == 5) std = 1;

    fileName = strcpy (fileName, argv[2]);
    fileOut = strcpy (fileOut, argv[3]);
    filaProcesso = criaFilaProcessos (fileName);

    /* Inicializa um semáforo com o valor de núcleos do pc */
    if(sem_init(&semNucleos, 0, numNucleos))
    {
        printf("Erro ao criar o semáforo!\n");
        exit(EXIT_FAILURE);
    }

    /* Inicializa um semáforo para alterar a fila de processos */
    if(sem_init(&semAlteraFila, 0, 1))
    {
        printf("Erro ao criar o semáforo!\n");
        exit(EXIT_FAILURE);
    }

    /* Inicializa um semáforo para alterar uso de seçoes criticas */
    if(sem_init(&semStd, 0, 1))
    {
        printf("Erro ao criar o semáforo!\n");
        exit(EXIT_FAILURE);
    }

    /* Selecao do escalonador*/

    switch (opt)
    {
    case 1:
        printf ("Argumento dado: 1 - Shortest Job First\n");
        printf("\n\nProcessando...\n");
        shortestJobFirst(fileOut);
        break;

    case 2:
        printf ("Argumento dado: 2 - Round Robin\n");
        printf("\n\nProcessando...\n");
        roundRobin(fileOut);
        break;

    case 3:
        printf ("Argumento dado: 3 - Escalonamento com Prioridade\n");
        printf("\n\nProcessando...\n");
        escalonamentoComPrioridade(fileOut);
        break;

    default:
        printf ("Argumento inválido.\n");
        return EXIT_FAILURE;
    }


    free (fileName);
    return EXIT_SUCCESS;
}

/* **** Escalonador 1 : Shortest Job First ******* */
void shortestJobFirst (char *outFile)
{
    Processo *coord = mallocSafe(sizeof(Processo));
    int control = 0;
    FILE *fp = fopen(outFile, "w");

    ordenaFilaSJF(filaProcesso);

    time(&start);

    /* Vai liberando cada thread no tempo t0 para ocupar a CPU*/
    for(coord = filaProcesso->prox; coord != NULL; coord = coord->prox)
        pthread_create(coord->thread, NULL, processoNaoPreemptivo,(void *) coord);

    while(control < numProc)
    {
        for(coord = filaProcesso->prox; coord != NULL; coord = coord->prox)
            if(coord->chegada == 1)
            {
                coord->chegada = 0;
                control ++;
                fprintf(fp, "%s %f %2f\n", coord->nome, coord->tf, coord->tr);
                if(std == 1)
                {
                    fprintf(stderr,"\n O processo %s foi finalizado e gravado na linha %d.\n",coord->nome,l++);
                    fprintf(stderr,"\n Quantidade atual de mudancas de contexto: %d.\n",0);
                }
            }
    }


    /*Destroi as threads*/
    for(coord = filaProcesso->prox; coord != NULL; coord = coord->prox)
        if (pthread_join(*coord->thread, NULL))
        {
            printf("\n ERROR joining thread");
            exit(1);
        }

    fprintf(fp, "%d\n", 0);
    fclose(fp);
}


/* **** Escalonador 2 : Round Robin  ******* */
void roundRobin (char *outFile)
{
    Processo *coord = mallocSafe(sizeof(Processo)), *aux;
    time_t now;
    double tf, tr,d;
    FILE *fp = fopen(outFile, "w");
    if(fp == NULL)
        printf("Erro, nao foi possivel abrir o arquivo de saida\n");

    /* Comecamos a contar o tempo*/
    time(&start);

    /* Enquanto a fila nao estier vazia*/
    while(filaProcesso->prox != NULL)
    {
        for(coord = filaProcesso->prox; coord != NULL; coord = coord->prox)
        {
            time(&now);

            /* Verifica se ja chegou no sistema*/
            if((d = difftime(now, start)) >= coord->t0)
            {
                if(coord->chegada == 0)
                {
                    coord->chegada = 1;
                    if(std == 1)
                        fprintf(stderr,"\n O processo %s foi iniciado. Esta na linha %d do trace.\n",coord->nome, coord->linha);
                }

                if(coord->ativo == 1)
                {
                    /*printf("processo entrando:%d\n", coord->id);*/
                    coord->dt = coord->dt-QUANTUM;
                    coord->ativo = 0;
                    pthread_create(coord->thread, NULL, processoPreemptivo,(void *) coord);

                    /* Rodou o tempo que precisava, entao sai da fila*/
                    if(coord->dt <= 0)
                    {
                        sem_wait (&semAlteraFila);

                        time(&now);
                        if(coord->dt == 0)
                            tf = difftime(now, start)+QUANTUM;
                        else
                            tf = difftime(now, start)+(-1*coord->dt);
                        tr = tf - coord->t0;

                        fprintf(fp, "%s %f %2f\n", coord->nome, tf, tr);

                        if(std == 1)
                        {
                            fprintf(stderr,"\n O processo %s foi finalizado e gravado na linha %d.\n",coord->nome,l++);
                            fprintf(stderr,"\n Quantidade atual de mudancas de contexto: %d.\n",numMudContexto);
                        }
                        aux = coord->ant;
                        coord->ant->prox = coord->prox;
                        if(coord->prox != NULL)
                            coord->prox->ant = coord->ant;
                        coord = aux;

                        sem_post(&semAlteraFila);
                    }
                }

            }
        }
    }
    fprintf(fp, "%d\n", numMudContexto);
    fclose(fp);
}


/* **** Escalonador 3 : Escalonamento com Prioridade ******* */
void escalonamentoComPrioridade (char *outFile)
{
    Processo *coord = mallocSafe(sizeof(Processo));
    int control = 0;
    FILE *fp = fopen(outFile, "w");

    ordenaFilaECP(filaProcesso);

    time(&start);


    /* Vai liberando cada thread no tempo t0 para ocupar a CPU*/
    for (coord = filaProcesso->prox; coord != NULL; coord = coord->prox)
        pthread_create (coord->thread, NULL, processoNaoPreemptivo, (void *) coord);

    while(control < numProc)
    {
        for(coord = filaProcesso->prox; coord != NULL; coord = coord->prox)
            if(coord->chegada == 1)
            {
                coord->chegada = 0;
                control ++;
                fprintf(fp, "%s %f %2f\n", coord->nome, coord->tf, coord->tr);
                if(std == 1)
                {
                    fprintf(stderr,"\n O processo %s foi finalizado e gravado na linha %d.\n",coord->nome,l++);
                    fprintf(stderr,"\n Quantidade atual de mudancas de contexto: %d.\n",0);
                }
            }
    }

    /* Destrói as threads */
    for (coord = filaProcesso->prox; coord != NULL; coord = coord->prox)
        if (pthread_join (*coord->thread, NULL))
        {
            printf ("\n ERROR joining thread");
            exit (EXIT_FAILURE);
        }

    fprintf(fp, "%d\n", 0);
    fclose(fp);
}


/* ********** Processamento e ordenacao do arquivo trace ******* */

Processo *criaFilaProcessos(char *fileName)
{
    FILE *fp;
    Processo *fila = NULL, *aux = NULL, *ant = NULL;
    int i = 1;

    fp = fopen (fileName, "r");

    if (fp == NULL)
    {
        printf ("Falha na abertura do arquivo de trace.\n");
        exit (EXIT_FAILURE);
    }
    else
    {
        fila = mallocSafe (sizeof (Processo)); /* cabeca da fila*/
        aux = mallocSafe (sizeof (Processo));
        fila->ant = NULL;
        fila->prox = aux;
        aux->ant = fila;
        fila->id = -1;

        while ((fscanf (fp,"%f %f %f %s\n",&aux->t0, &aux->dt, &aux->deadline, aux->nome)) != EOF)
        {
            aux->thread = mallocSafe (sizeof (pthread_t));
            aux->linha = i;
            aux->tf = 0;
            aux->tr = 0;
            aux->ativo = 1;
            aux->chegada = 0;
            aux->id = i++;
            aux->prox = mallocSafe (sizeof (Processo));
            ant = aux;
            aux = aux->prox;
            aux->ant = ant;
            numProc++;
        }
        free(aux);
        ant->prox = NULL;
    }
    fclose(fp);

    return fila;
}


/* **** Ordenacao para SJF ********* */
void ordenaFilaSJF(Processo *fila)
{
    float atual;
    Processo *ini = fila->prox, *fim = fila->prox;

    atual = ini->t0;

    while(fim->prox != NULL)
    {

        while(fim->t0 == atual && fim->prox != NULL)fim = fim->prox;

        if(fim->t0 != atual)
        {
            ordenarSubLista(ini,fim->ant);
            ini = fim;
            atual = ini->t0;
        }
        else
            ordenarSubLista(ini,fim);
    }
}


/* **** SubOrdenacao para SJF ********* */
void ordenarSubLista(Processo *inicio, Processo *fim)
{
    Processo *i, *j, *anterior = inicio->ant;


    for(i = inicio->prox; i != fim->prox; i = i->prox)
    {
        for(j = i->ant; j != anterior; j = j->ant)
        {
            /* Troca dois processos na fila */
            if((i->dt) < (j->dt))
            {
                j->ant->prox = i;
                i->ant = j->ant;
                j->ant = i;
                j->prox = i->prox;
                i->prox = j;
                if(j->prox != NULL)
                    j->prox->ant = j;
            }
        }
    }
}

/* **** SubOrdenacao para escalonamento com prioridade ********* */
void ordenarSubLista2 (Processo *inicio, Processo *fim)
{
    Processo *i, *j, *anterior = inicio->ant;

    for (i = inicio->prox; i != fim->prox; i = i->prox)
    {
        for (j = i->ant; j != anterior; j = j->ant)
        {
            /* Troca dois processos na fila */
            if((i->deadline) < (j->deadline))
            {
                j->ant->prox = i;
                i->ant = j->ant;
                j->ant = i;
                j->prox = i->prox;
                i->prox = j;
                if (j->prox != NULL)
                    j->prox->ant = j;
            }
        }
    }
}


/* **** Ordenacao para escalonamento com prioridade ********* */
void ordenaFilaECP (Processo *fila)
{
    float atual;
    Processo *ini = fila->prox, *fim = fila->prox;

    atual = ini->t0;

    while (fim->prox != NULL)
    {
        while (fim->t0 == atual && fim->prox != NULL) fim = fim->prox;
        if (fim->t0 != atual)
        {
            ordenarSubLista2 (ini, fim->ant);
            ini = fim;
            atual = ini->t0;
        }
        else
            ordenarSubLista2 (ini, fim);
    }
}

/* **** Checagem da fila de processos  ********* */
void verFila (Processo *fila)
{
    Processo *aux = fila->prox;

    printf("**************\n");

    while (aux != NULL)
    {
        printf ("Processo id: %d\n\n", aux->id);
        printf ("t0:%f dt:%f deadline:%f nome:%s\n\n",aux->t0, aux->dt, aux->deadline, aux->nome);
        printf ("Processo anterior id: %d\n\n", aux->ant->id);
        if(aux->prox != NULL)
            printf ("Processo proximo id: %d\n\n", aux->prox->id);
        else
            printf ("NULL\n\n");
        printf ("--------------\n");
        aux = aux->prox;
    }
}


/*  ******** Calcula a quantidade de nucleos existente no computador ****** */
int calculaNucleos ()
{
    FILE *f;
    int i;
    char *c = mallocSafe (3 * sizeof (char));

    if(system ("nproc > nproc_temp"))
    {
        printf ("Erro ao calcular número de núcleos!\n");
        exit (EXIT_FAILURE);
    }
    f = fopen ("nproc_temp", "r");
    if (fgets (c, 3, f) == NULL)
    {
        printf ("Erro ao calcular número de núcleos!\n");
        exit (EXIT_FAILURE);
    }
    i = atoi (c);
    fclose (f);
    free (c);
    remove ("nproc_temp");

    return i;
}

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



/********************************* FUNCOES DE PROCESSOS **************************/

/* Gerencia o comportamento das threads */
void *processoNaoPreemptivo (void *param)
{
    Processo *p = ((Processo *) (param));
    int retval = 0;
    time_t now;

    usleep(p->t0*1000000);

    if(std == 1)
        fprintf(stderr,"\n O processo %s foi iniciado. Esta na linha %d do trace.\n",p->nome, p->linha);

    /*Seção Critica*/
    sem_wait (&semNucleos); /* Mesmo que P(semNucleos) */

    if (std == 1)
    {
        sem_wait (&semStd);
        if(sem_getvalue(&semNucleos, &retval) == 0)
            fprintf(stderr,"\n O processo %s ocupou a CPU %d.\n",p->nome,retval+1);
        else
            fprintf(stderr," Falha ao identificar a CPU\n");

        sem_post (&semStd);
    }


    ocupaCPU(p->dt);

    time(&now);

    p->tf = difftime(now, start);
    p->tr = p->tf - p->t0;
    p->chegada = 1;

    sem_post (&semNucleos); /* Mesmo que V(semNucleos) */

    if(std == 1)
    {
        fprintf(stderr,"\n O processo %s foi finalizado e gravado na linha %d.\n",p->nome,l++);
        fprintf(stderr,"\n Quantidade atual de mudancas de contexto: %d.\n",0);
    }

    return NULL;
}

void *processoPreemptivo (void *param)
{

    Processo *p = ((Processo *) (param));
    float tempo = 0;
    int retval = 0;

    if(p->dt >= 0)tempo = QUANTUM;
    else tempo = (-1)*p->dt;

    /*Seção Critica*/
    sem_wait (&semNucleos); /* Mesmo que P(semNucleos) */

    if (std == 1)
    {
        sem_wait (&semStd);
        if(sem_getvalue(&semNucleos, &retval) == 0) /* teste */
            fprintf(stderr,"\n O processo %s ocupou a CPU %d.\n",p->nome,retval+1);
        else
            fprintf(stderr," Falha ao identificar a CPU\n");

        fprintf(stderr,"\n O processo %s deixou a CPU %d.\n",p->nome,retval+1);


        sem_post (&semStd);
    }

    sem_wait (&semStd);
    numMudContexto++;
    sem_post (&semStd);

    ocupaCPU(tempo);

    p->ativo = 1;

    sem_post (&semNucleos); /* Mesmo que V(semNucleos) */

    return NULL;
}

void ocupaCPU(float tempo)
{
    float t = tempo;

    for(t = tempo; t > 0; t = t - 0.1) usleep(0.1*1000000);

}
