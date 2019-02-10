/* ******************************************************************* */
/*                                                                     */
/*                 MAC0422 - Sistemas Operacionais                     */
/*                       Exercício-programa 2                          */
/*                                                                     */
/*              Bárbara de Castro Fernandes - 7577351                  */
/*            Taís Aparecida Pereira Pinheiro - 7580421                */
/*                                                                     */
/*                       Arquivo: ciclista.c                           */
/* ******************************************************************* */

#include <stdlib.h>
#include <unistd.h>
#include "ciclista.h"
#include "pista.h"
#include "aux.h"



int ciclistasAtivos()
{
    return nCiclistasAtivos;
}


void iniciaD(int d)
{
    dp = d;
}

void escolheCiclistaAleatorio()
{
    int cic, random = rand ()%10;

    if (random == 0)
    {
        cic = (random/RAND_MAX)*nCiclistasAtivos;
        ciclistas[cic].fast = 1;
        muda = 1;
    }
}



/* A pontuação do ciclista é atualizada a cada 10 voltas que ele dá. */
void atualizaPontuacao()
{
    int i;

    for(i = 0; i < 4; i++)
    {
        if(i == 0) ciclistas[i].pontuacao += 5;
        else if(i == 1) ciclistas[i].pontuacao += 3;
        else if(i == 2) ciclistas[i].pontuacao += 2;
        else ciclistas[i].pontuacao += 1;
    }
}

void inicializaCiclistas (int num)
{
    int i;

    nCiclistasAtivos = num;
    n = num;

    ciclistas = mallocSafe(n * sizeof(Ciclista));

    for (i = 0; i < n; i++)
    {
        ciclistas[i].id = i;
        ciclistas[i].v = 0;
        ciclistas[i].vAntes = 0;
        ciclistas[i].volta = 0;
        ciclistas[i].ativo = 1;
        ciclistas[i].pontuacao = 0;
        ciclistas[i].livre = 1;
        ciclistas[i].colocacao = i;
        ciclistas[i].tempoVoltaAtual = 0;
        ciclistas[i].fast = 0;
    }
}


void mostraCiclistas()
{
    int i;

    for (i = 0; i < n; i++)
    {
        printf("------------\n");
        printf("id:%d | ",ciclistas[i].id);
        printf("v:%d  | ",ciclistas[i].v);
        printf("vAntes:%d  | ",ciclistas[i].vAntes);
        printf("volta:%d  | ",ciclistas[i].volta);
        printf("ativo:%d  | ",ciclistas[i].ativo);
        printf("pont:%d  | ",ciclistas[i].pontuacao);
        printf("livre:%d | ",ciclistas[i].livre);
        printf("tempo:%f | ",ciclistas[i].tempoVoltaAtual);
        printf("colocacao:%d | ",ciclistas[i].colocacao);
        printf("\n");
    }

}


void defineVelocidadeCiclista (int pos)
{
    int random = rand ();

    if (ciclistas[pos].v == 30)
    {
        if (random <= RAND_MAX * 0.7)
        {
            ciclistas[pos].v = 60;
            ciclistas[pos].vAntes = 30;
        }
        else
        {
            ciclistas[pos].v = 30;
            ciclistas[pos].vAntes = 30;
        }
    }
    else
    {
        if (random <= RAND_MAX * 0.5)
        {
            ciclistas[pos].v = 60;
            ciclistas[pos].vAntes = 60;
        }
        else
        {
            ciclistas[pos].v = 30;
            ciclistas[pos].vAntes = 60;
        }
    }
}

int quebra()
{
    int random = rand ()%10;

    if (random == 0) return 0;
    else  return 0;
}

int ranking(int pos)
{
    int i, p = 0;

    for(i = 0; i < n; i++)
        if(ciclistas[i].pontuacao >= ciclistas[pos].pontuacao) p++;

    return p;
}

void eliminaCiclista(int pos)
{
    /* P em Print*/
    sem_wait(&semPrint);

    nCiclistasAtivos--;
    printf("\n\n------> Ciclista quebrou!\n");
    printf("id: %d\n",ciclistas[pos].id);
    printf("volta: %d\n",ciclistas[pos].volta+1);
    printf("Colocao em pontos: %d\n",ranking(pos));
    /* V em Print*/
    sem_post(&semPrint);
}

void *volta(void *param)
{
    int pos = *((int *)(param));
    int i,j,livre = 0;
    time_t ini,fim;

    /* Determina ciclista e suas velocidades de inicio*/
    inicioVolta(pos);

    /* Espera todo mundo chegar*/
    pthread_barrier_wait(&iniVolta);

    /* Se o ciclista continua ativo*/
    if(ciclistas[pos].ativo == 1)
    {
        /* Espera para entrar na pista*/
        usleep((pos/10)*0.06*1000000);

        /*printf("eu %d esperei o tempo de entrar\n",ciclistas[pos].id);*/
        /* Pega a posicao da coluna atual na pista*/
        j = pos%10;

        /*printf("eu %d defini a minha raia\n",ciclistas[pos].id);*/
        time(&ini);

        /* Entra na pista e percorre as d linhas*/
        for(i = 0; i < dp; i++)
        {
            /* Retira seu id da posicao anterior*/
            if(i > 0)
            {
                sem_wait (&local[i-1][j]);
                pista[i-1][j] = -1;
                sem_post (&local[i-1][j]);
            }

            /* Cisclista espera enquanto nao pode assumir uma posicao*/
            /* Espera ou tenta uma ultrapassagem*/
            /* Se acontecer ultrapassagem, atualizar o j*/
            while(livre != 1)
            {
                livre = posicaoLivre(i,j);
                /* Por um motivo que so Deus sabe, se tirar esse print nao funciona*/
                printf("\n");
            }
            livre = 0;

            /* Quando disponivel, assume a nova posicao*/
            sem_wait (&local[i][j]);
            pista[i][j] = ciclistas[pos].id;
            sem_post (&local[i][j]);

            /* espera um tempo dependendo da velocidade*/

            /* Se foi sorteado para corre a 90km*/
            if(ciclistas[pos].fast == 1) usleep(0.04*1000000);
            /* Se a velocidade for 30*/
            else if(ciclistas[pos].v == 30) usleep(2*0.06*1000000);
            /* Se a velocidade for 60*/
            else usleep(0.06*1000000);

            /* Se chegou na ultima linha, libera para quem vem atras*/
            if(i == dp-1)
            {
                sem_wait (&local[i][j]);
                pista[i][j] = -1;
                sem_post (&local[i][j]);
            }
        }
        time(&fim);
        ciclistas[pos].tempoVoltaAtual = difftime(fim, ini);
        ciclistas[pos].chegada = fim;
        ciclistas[pos].volta++;
    }
    if(pthread_barrier_wait(&fimVolta) == PTHREAD_BARRIER_SERIAL_THREAD)
        segue = 1;

    return NULL;
}


/* Gerencia o comportamento das threads no inicio de cada volta*/
void inicioVolta(int p)
{
    int pos = p;
    int q = 0;

    if(ciclistas[pos].ativo == 1)
    {
        /* Se vai ser a 1 volta*/
        if(ciclistas[pos].volta == 0)
            ciclistas[pos].v = 30;
        else
        {
            /* Verifica possibilidade de quebra*/
            if((ciclistas[pos].volta%15) == 0)
            {
                if(nCiclistasAtivos > 5)
                {
                    q = quebra();
                    if(q == 1)
                    {
                        pthread_t dummy_ini;
                        pthread_t dummy_fim;
                        ciclistas[pos].ativo = 0;
                        pthread_create(&dummy_ini, NULL, dummyThread,(void *) &iniVolta);
                        pthread_create(&dummy_fim, NULL, dummyThread,(void *) &fimVolta);
                        eliminaCiclista(pos);
                        pthread_exit(NULL);

                    }
                }
            }
            /* Se nao quebrou*/
            if(ciclistas[pos].ativo == 1)
            {
                /* P em semVel*/
                sem_wait(&semVel);

                /*define a velocidade se possivel*/
                if(ciclistas[pos].livre == 0)
                {
                    ciclistas[pos].livre = 1;
                    ciclistas[pos].v = 30;
                }
                else
                {
                    /*define a velocidade*/
                    defineVelocidadeCiclista(pos);

                    /*define a velocidade de todos atras*/
                    if(ciclistas[pos].v == 30)
                    {
                        defineVelocidadeLinha(pos);
                    }
                }

                /* V em semVel*/
                sem_post(&semVel);
            }
        }
    }
}

/* Assumindo que o vetor de ciclista sempre est'a ordenado por colocacao*/

void defineVelocidadeLinha(int colocacao)
{
    int i;

    for (i = colocacao + 10; i < n; i = i + 10)
    {
        ciclistas[i].v = 30;
        ciclistas[i].livre = 0;
    }
}

/* Inicializa variaveis de controle dos ciclistas*/
void iniciaControladores()
{
    segue = 0;
    /*iniVolta = mallocSafe(sizeof(pthread_barrier_t));*/
    if(pthread_barrier_init(&iniVolta, NULL, n))
    {
        printf("\nErro na criação da barreira!\n");
        exit(EXIT_FAILURE);
    }
    /*fimVolta = mallocSafe(sizeof(pthread_barrier_t));*/
    if(pthread_barrier_init(&fimVolta, NULL, n))
    {
        printf("\nErro na criação da barreira!\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_barrier_init(&temp, NULL, n))
    {
        printf("\nErro na criação da barreira!\n");
        exit(EXIT_FAILURE);
    }

    if(sem_init(&semVel, 0, 1))
    {
        printf("Erro ao criar o semáforo velocidade!\n");
        exit(EXIT_FAILURE);
    }

    if(sem_init(&semPrint, 0, 1))
    {
        printf("Erro ao criar o semáforo de Impressao!\n");
        exit(EXIT_FAILURE);
    }
    if(sem_init(&tela, 0, 1))
    {
        printf("Erro ao criar o semáforo de Impressao!\n");
        exit(EXIT_FAILURE);
    }
}

int seguir()
{
    return segue;
}

int posicaoLivre(i,j)
{
    if(pista[i][j] == -1) return 1;
    return 0;
}


void organizaCiclistas()
{
    int i,j;
    double k;
    Ciclista x;


    for (j = 1; j < n; ++j)
    {
        k = ciclistas[j].tempoVoltaAtual;
        x.ativo = ciclistas[j].ativo;
        x.id = ciclistas[j].id;
        x.livre = ciclistas[j].livre;
        x.pontuacao = ciclistas[j].pontuacao;
        x.tempoVoltaAtual = ciclistas[j].tempoVoltaAtual;
        x.v = ciclistas[j].v;
        x.vAntes = ciclistas[j].vAntes;
        x.volta = ciclistas[j].volta;
        x.chegada = ciclistas[j].chegada;

        for (i = j-1; i >= 0 && ciclistas[i].tempoVoltaAtual > k; --i)
        {
            ciclistas[i+1].ativo = ciclistas[i].ativo;
            ciclistas[i+1].id = ciclistas[i].id;
            ciclistas[i+1].livre = ciclistas[i].livre;
            ciclistas[i+1].pontuacao = ciclistas[i].pontuacao;
            ciclistas[i+1].tempoVoltaAtual = ciclistas[i].tempoVoltaAtual;
            ciclistas[i+1].v = ciclistas[i].v;
            ciclistas[i+1].vAntes = ciclistas[i].vAntes;
            ciclistas[i+1].volta = ciclistas[i].volta;
            ciclistas[i+1].chegada = ciclistas[i].chegada;
        }

        ciclistas[i+1].ativo = x.ativo;
        ciclistas[i+1].id = x.id;
        ciclistas[i+1].livre = x.livre;
        ciclistas[i+1].pontuacao = x.pontuacao;
        ciclistas[i+1].tempoVoltaAtual = x.tempoVoltaAtual;
        ciclistas[i+1].v = x.v;
        ciclistas[i+1].vAntes = x.vAntes;
        ciclistas[i+1].volta = x.volta;
        ciclistas[i+1].chegada = x.chegada;
    }
}


void ajustaPosicoes()
{
    int i,j;
    Ciclista *aux;

    aux = mallocSafe(n*sizeof(Ciclista));

    /* Copia os ativos*/
    for (i = 0, j = 0; i < n; i++)
        if(ciclistas[i].ativo == 1)
        {
            aux[j].ativo = ciclistas[i].ativo;
            aux[j].id = ciclistas[i].id;
            aux[j].livre = ciclistas[i].livre;
            aux[j].pontuacao = ciclistas[i].pontuacao;
            aux[j].tempoVoltaAtual = ciclistas[i].tempoVoltaAtual;
            aux[j].v = ciclistas[i].v;
            aux[j].vAntes = ciclistas[i].vAntes;
            aux[j].volta = ciclistas[i].volta;
            aux[j].chegada = ciclistas[i].chegada;
            j++;
        }
    /* Copia os inativos*/
    for (i = 0; i < n; i++)
        if(ciclistas[i].ativo == 0)
        {
            aux[j].ativo = ciclistas[i].ativo;
            aux[j].id = ciclistas[i].id;
            aux[j].livre = ciclistas[i].livre;
            aux[j].pontuacao = ciclistas[i].pontuacao;
            aux[j].tempoVoltaAtual = ciclistas[i].tempoVoltaAtual;
            aux[j].v = ciclistas[i].v;
            aux[j].vAntes = ciclistas[i].vAntes;
            aux[j].volta = ciclistas[i].volta;
            aux[j].chegada = ciclistas[i].chegada;
            j++;
        }
    /* Copia todo mundo na ordem certa*/
    for (i = 0; i < n; i++)
    {
        ciclistas[i].ativo = aux[i].ativo;
        ciclistas[i].id = aux[i].id;
        ciclistas[i].livre = aux[i].livre;
        ciclistas[i].pontuacao = aux[i].pontuacao;
        ciclistas[i].tempoVoltaAtual = aux[i].tempoVoltaAtual;
        ciclistas[i].v = aux[i].v;
        ciclistas[i].vAntes = aux[i].vAntes;
        ciclistas[i].volta = aux[i].volta;
        ciclistas[i].chegada = aux[i].chegada;
        ciclistas[i].colocacao = i;
    }

    free(aux);
}

void tempoDeChegada()
{
    int i;

    /* Ajusta o empo de chegada para cada ciclista*/
    for (i = 0; i < n; i++)
    {
        if(ciclistas[i].ativo == 1)
            ciclistas[i].chegada =  difftime(ciclistas[i].chegada,start);
    }
}


void *dummyThread(void *param)
{
    pthread_barrier_t barr = *((pthread_barrier_t *)(param));

    while(1)
        pthread_barrier_wait(&barr);


    return NULL;
}


/* Utilizada para opcao debug*/
void *show(void *m)
{
    while(muda == 0)
    {
        usleep(0.06*1000000);
        sem_wait (&tela);
        mostraPista(dc);
        usleep(0.50*1000000);
        sem_post (&tela);
    }
    while(muda == 1)
    {
        usleep(0.02*1000000);
        sem_wait (&tela);
        mostraPista(dc);
        usleep(0.50*1000000);
        sem_post (&tela);
    }

    return NULL;
}

