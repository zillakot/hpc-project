#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "genetic.h"
#include "cities.h"
#include "mpi.h"

int main(int argc, char *argv[]) {
	int n;
	
	double** distances;
	Config config;
	
	MPI_Status status;
  	int myid, size, tag=1, rc;
	
	/* Start up MPI */
  	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
	    printf ("Error starting MPI program. Terminating.\n");
	    MPI_Abort(MPI_COMM_WORLD, rc);
	}
	
  	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(myid==0){
		/*Init cities*/
		FILE* fptr = fopen("input.in", "r");
		assert(fptr != NULL);
	
		n =	count_lines(fptr);
		printf("\nTaskID %i: Number of cities: %i\n", myid,n);
	
		City* cities[n];
		read_cities(fptr, cities);
		printf("TaskID %i: Init cities succesful... \n", myid);
	
		/*Init distances*/
		distances=calculate_distances(distances,cities, n);
		MPI_Bcast(&distances, sizeof(double**), MPI_BYTE, 0, MPI_COMM_WORLD);
		printf("TaskID %i: Init distances succesful... \n", myid);
		
		/*Init GA config*/
		config.populationSize=1000/size;
		config.mutationRate=0.2;
		config.numGenerations=1000;
		config.numElitism=1;
		config.mutationSize=1;
		config.maxBreeding=10;
		config.numGenes=n;
		MPI_Bcast(&config, sizeof(Config), MPI_BYTE, 0, MPI_COMM_WORLD);
		printf("TaskID %i: Init GA config succesful... \n", myid);
	}
	
	/*Init random seed*/
	srand ( time(NULL) );
	printf("TaskID %i: Init random seed succesful... \n", myid);

	/*Init population*/
   	Population population;
	generate_random_population(&population, distances, &config);
	printf("TaskID %i: Init population succesful... \n", myid);
	
	int numGenerations;
	/*Start evolution*/
	for(numGenerations=1; numGenerations < config.numGenerations; numGenerations++){
		
		/*Sort population*/
		qsort(population.path, population.numPaths, sizeof(Path), compare_population);
		
		/*Print best choises for every 100 generations*/
		if (numGenerations%100==0) {
			printf("%i. generation\n", numGenerations);
			printf("TaskID: %i, bestfit: %i; ", myid, population.path[0].fitness);
		}
			
		/*Breed population*/
		simple_breed_population(&population, config.numGenes, distances);
		
		/*Send and receive child of best choises from other tasks for every 10 generations*/
		if (numGenerations%10==0) {
			MPI_Sendrecv(&population.path[config.populationSize-1], sizeof(Path), MPI_BYTE, (myid+1)%size, tag,
						 &population.path[config.populationSize-1], sizeof(Path), MPI_BYTE, (myid+1)%size, tag, 
						 MPI_COMM_WORLD,&status);
		}
		
		/*Mutate population*/
		mutate_population(&population, distances, &config);
		
	}
	
	
	
	/*if(myid == 0){
		int j;
		Path shortest;
		shortest.fitness=population.path[0].fitness;
		
		for(j=1; j < size; j++){
			MPI_Recv(&population.path[0], sizeof(Path), MPI_BYTE, j, tag, MPI_COMM_WORLD, &status);
			if(population.path[0].fitness < shortest.fitness) shortest.fitness = population.path[0].fitness;
		}
		printf("\nShortest path: %i", shortest.fitness);
	} else {
		MPI_Send(&population.path[0], sizeof(Path), MPI_BYTE, 0, tag, MPI_COMM_WORLD);
	}*/
	
	
	MPI_Finalize();
	
return 0;
}





