# makefile

CC = gcc -lm -std=c99 -pedantic -Wall -Wextra -Werror

all: TSP
TSP: tsp_genetic.o cities.o genetic.o 
	$(CC) tsp_genetic.o cities.o genetic.o -o TSP
cities.o: cities.c
	$(CC) -c cities.c -o cities.o
genetic.o: genetic.c
	$(CC) -c genetic.c -o genetic.o
tsp_genetic.o: tsp_genetic.c
	$(CC) -c tsp_genetic.c -o tsp_genetic.o

clean:
	rm *.o
	rm TSP