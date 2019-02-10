# ################################################################### #
#                                                                     #
#                 MAC0422 - Sistemas Operacionais                     #
#                       Exercício-programa 2                          #
#                                                                     #
#              Bárbara de Castro Fernandes - 7577351                  #
#            Taís Aparecida Pereira Pinheiro - 7580421                #
#                                                                     #
#                         Arquivo: Makefile                           #
# ################################################################### #

CC = gcc
CFLAGS = -O2 -Wall -pedantic -pthread
OBJS = aux.o ciclista.o pista.o ep2.o
LIBS = aux.h ciclista.h pista.h

#all: ep2-barbara-tais

ep2: $(OBJS)
	$(CC) $(OBJS) -o $@ $(CFLAGS)

%.o: %.c $(LIBS)
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f ep2 *.o *~

