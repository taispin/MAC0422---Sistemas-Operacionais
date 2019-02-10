/* ******************************************************************* */
/*                                                                     */
/*                 MAC0422 - Sistemas Operacionais                     */
/*                       Exercício-programa 2                          */
/*                                                                     */
/*              Bárbara de Castro Fernandes - 7577351                  */
/*            Taís Aparecida Pereira Pinheiro - 7580421                */
/*                                                                     */
/*                       Arquivo: ciclista.h                           */
/* ******************************************************************* */

#ifndef _CICLISTA_H_
#define _CICLISTA_H_
#include <semaphore.h>
#include <pthread.h>

#define MAX_CICLISTAS_EMPARELHADOS 10

typedef struct ciclista {
    int id;
    int v;
    int vAntes;
    int colocacao;
    int volta;
    int ativo;
    int pontuacao;
    int livre;
    int fast;
    double tempoVoltaAtual;
    double chegada;
    pthread_t thread;
} Ciclista;

Ciclista *ciclistas; /* QUESTION: e se esta estrutura fosse uma lista ligada? */
int n, dp, muda;
sem_t semVel; /* semaforo para defonicao da velocidade*/
sem_t semPrint; /* semaforo para controle de impressao*/
pthread_barrier_t iniVolta; /* barreira de sincronizacao*/
pthread_barrier_t fimVolta; /* barreira de sincronizacao*/
pthread_barrier_t temp; /* barreira de sincronizacao*/
int nCiclistasAtivos;
int segue;
time_t start;


void avancaTodosCiclistas ();
void inicializaCiclistas ();
void mostraCiclistas();
void inicioVolta(int p);
void defineVelocidadeLinha(int colocacao);
int quebra();
void eliminaCiclista(int pos);
int ranking(int pos);
void iniciaControladores();
void reIniciaControladores();
void *percurso(void *param);
int ciclistasAtivos();
void iniciaD(int d);
void *volta(void *param);
int seguir();
int posicaoLivre(int i, int j);
void organizaCiclistas();
void ajustaPosicoes();
void atualizaPontuacao();
void escolheCiclistaAleatorio();
void tempoDeChegada();
void escolheCiclistaAleatorio();
void *dummyThread(void *param);
void *show(void *m);

#endif
