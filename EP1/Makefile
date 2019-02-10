# ################################################################### #
#                                                                     #
#                 MAC0422 - Sistemas Operacionais                     #
#                       Exercício-programa 1                          #
#                                                                     #
#              Bárbara de Castro Fernandes - 7577351                  #
#            Taís Aparecida Pereira Pinheiro - 7580421                #
#                                                                     #
# ################################################################### #


CC = gcc

CFLAGS = -O2 -Wall -pedantic -pthread -lreadline

READLINE    = -DREADLINE_SUPPORT

READLINELIB = -L/usr/local/lib  -lcurses

all: ep1sh ep1

ep1sh: ep1sh.c
	$(CC) $< -o $@ $(CFLAGS)

ep1: ep1.c
	$(CC) $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f ep1sh ep1 *~
