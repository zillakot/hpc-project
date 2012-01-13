#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "genetic.h"
#include "cities.h"
#include "mpi.h"

int main(int argc, char *argv[]) 
	int argc;
	char *argv[]; {
	int n,i;
	
	double** distances;
	Config config;
	
	MPI_Status status;
  	int num, myid, size, tag=1, rc;
	
	/* Start up MPI */
  	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
	    printf ("Error starting MPI program. Terminating.\n");
	    MPI_Abort(MPI_COMM_WORLD, rc);
	}
	
  	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);
	printf("Init MPI succesful... \n");
	
	if(myid==0){
		/*Init cities*/
		FILE* fptr = fopen("input.in", "r");
		assert(fptr != NULL);
	
		n =	count_lines(fptr);
		printf("\nNumber of cities: %i\n",n);
	
		City* cities[n];
		read_cities(fptr, cities);
		printf("Init cities succesful... \n");
	
		/*Init distances*/
		distances=calculate_distances(distances,cities, n);
		MPI_Bcast(distances, sizeof(double**), MPI_BYTES, 0, MPI_COMM_WORLD);
		printf("Init distances succesful... \n");
		
		/*Init GA config*/
		config.populationSize=1000/size;
		config.mutationRate=0.1;
		config.numGenerations=1000;
		config.numElitism=1;
		config.mutationSize=1;
		config.maxBreeding=10;
		config.numGenes=n;
		MPI_Bcast(config, sizeof(Config), MPI_BYTES, 0, MPI_COMM_WORLD);
		printf("Init GA config succesful... \n");
	}
	
	/*Init random seed*/
	srand ( time(NULL) );
	printf("Init random seed succesful... \n");

	/*Init population*/
   	Population population;
	generate_random_population(&population, distances, &config);
	printf("Init population succesful... \n");

	/*Start evolution*/
	for(numGenerations=1; numGenerations < config.numGenerations; numGenerations++){
		
		/*Sort population*/
		qsort(population.path, population.numPaths, sizeof(Path), compare_population);
		
		/*Print best choises for every 100 generations*/
		if (numGenerations%100==0) {
			printf("%i. generation\n", numGenerations);
			printf("taskID: %i, bestfit: %i; ", myid, population.path[0].fitness);
		}
			
		/*Breed population*/
		simple_breed_population(&population, config.numGenes, distances);
		
		/*Send and receive child of best choises from other tasks for every 10 generations*/
		if (numGenerations%10==0) {
			MPI_Sendrecv(population.path[config.populationSize-1], sizeof(Path), MPI_BYTES, (myid+1)%size, tag,
						 population.path[config.populationSize-1], sizeof(Path), MPI_BYTES, (myid+1)%size, tag, 
						 mpi_comm_world,status,rc);
		}
		
		/*Mutate population*/
		mutate_population(&population, &config);
		
	}
	
	
	
	if(myid == 0){
		int j;
		Path shortest;
		shortest.fitness=population.path[0].fitness;
		
		for(j=1; j < size; j++){
			MPI_Recv(population.path[0], sizeof(Path), MPI_BYTES, j, tag, MPI_COMM_WORLD, &Stat);
			if(population.path[0].fitness < shortest.fitness) shortest.fitness = population.path[0].fitness;
		}
		printf("\nShortest path: %i"shortest.fitness);
	} else {
		MPI_Send(population.path[0], sizeof(Path), MPI_BYTES, 0, tag, MPI_COMM_WORLD);
	}
	
	
	MPI_Finalize();
	
return 0;
}





