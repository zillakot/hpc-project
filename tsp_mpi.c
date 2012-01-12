#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "genetic.h"
#include "cities.h"
#include "mpi.h"

int main(int argc, char *argv[]) {
	int n;
	
	MPI_Status status;
  	int num, rank, size, tag, next, from;

  	/* Start up MPI */

  	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	/*Init cities*/
	FILE* fptr = fopen("input.in", "r");
	assert(fptr != NULL);
	
	n =	count_lines(fptr);
	printf("\nNumber of cities: %i\n",n);
	
	City* cities[n];
	read_cities(fptr, cities);
	printf("Init cities succesful... \n");
	
	/*Init distances*/
	double** distances;
	distances=calculate_distances(distances,cities, n);
	printf("Init distances succesful... \n");
	
	/*Init GA config*/
	Config config;
	config.populationSize=1000;
	config.mutationRate=0.1;
	config.numGenerations=1000;
	config.numElitism=1;
	config.mutationSize=1;
	config.maxBreeding=10;
	config.numGenes=n;
	printf("Init GA config succesful... \n");
	
	/*Init random seed*/
	srand ( time(NULL) );
	printf("Init random seed succesful... \n");
	
	/*Init population*/
    Population population;
	generate_random_population(&population, distances, &config);
	printf("Init population succesful... \n");

    int numGenerations = 0;
	printf("Init mostFit succesful... \n");
	
	/*Start evolution*/
	while(numGenerations < config.numGenerations){
		numGenerations++;
		
		if (numGenerations%10==0) printf("%i. generation", numGenerations);
		
		/*Sort population*/
		qsort(population.path, population.numPaths, sizeof(Path), compare_population);
			
		/*Breed population*/
		simple_breed_population(&population, config.numGenes, distances);
		
		/*Mutate population*/
		mutate_population(&population, &config);
		
		
	}
	
	print_population(&population);
	
return 0;
}





