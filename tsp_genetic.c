#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "genetic.h"
#include "cities.h"

int main(void) {
	int n;
	FILE* fptr = fopen("input.in", "r");
	assert(fptr != NULL);
	
	n =	count_lines(fptr);
	printf("\nNumber of cities: %i\n",n);
	
	/*Init cities*/
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
	config.numGenerations=6000;
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
		
		if (numGenerations%100==0) printf("%i. generation", numGenerations);
		
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





