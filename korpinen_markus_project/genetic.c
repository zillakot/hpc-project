#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "genetic.h"
#include "cities.h"

int generate_random_population(Population *population, double** distances, Config *config){
	int i;

	population->numPaths = config->populationSize;

	if((population->path = (Path*)malloc(sizeof(Path) * population->numPaths)) == NULL){
	    return 1;
	}
	for(i=0; i<population->numPaths; i++){
		if((population->path[i].combination = malloc((config->numGenes)*sizeof(short unsigned int))) == NULL){
	        return 1;
		}
	}
	for(i=0; i<population->numPaths; i++){
		generate_random_combination(&population->path[i], config);
	    calculate_fitness(&population->path[i], config, distances);
	    population->path[i].breedChance = 0.0;
	}

	return 0;
}
	
int generate_random_combination(Path *path, Config *config){
	int isAlreadyUsed[config->numGenes];
	short unsigned int chosenNode = 0;
	int i;

	for(i=0; i<config->numGenes; i++){
	        isAlreadyUsed[i] = 0;
	}
	for(i=0; i<config->numGenes; i++){

        // choose random node of the map
        //chosenNode = lrand48() % config->numGenes;
        chosenNode = rand() % config->numGenes;
        //if this node was chosen already
        if(isAlreadyUsed[chosenNode] != 0){

	        //choose the next one of the list
	        while(isAlreadyUsed[chosenNode] != 0){
	            if(chosenNode + 1 < config->numGenes){
	            	chosenNode++;
	            }else{
	            	chosenNode = 0;
	            }
	        }
        }
        isAlreadyUsed[chosenNode] = 1;
        path->combination[i] = chosenNode;
	}

	return 0;
}
int calculate_fitness(Path *p, Config *config, double** distances){
	int fitness = 0;
	int i;
	int currentNode, nextNode;
	for(i=0; i<config->numGenes-1; i++){
		currentNode = p->combination[i];
		nextNode = p->combination[i+1];
		fitness += distances[currentNode][nextNode];
	}

	currentNode = p->combination[config->numGenes-1];
	nextNode = p->combination[0];
	fitness += distances[currentNode][nextNode];

	p->fitness = fitness;
	return 0;
}

/* qsort function*/
int compare_population(const void * a, const void * b){
	Path *a1 = (Path*)a;
	Path *b1 = (Path*)b;

	return (a1->fitness - b1->fitness);
}

void print_population(Population *population){
	int i;
	for(i=0;i<population->numPaths;i++){
		printf("n:%i, fitness:%i\n",i,population->path[i].fitness);
	}
	
}

void simple_breed_population(Population *p, int n, double** distances){
	int i;
	for(i=0;i<10;i++){
		p->path[p->numPaths-i-1]=mate(&p->path[i*2], &p->path[i*2+1], n, distances);
	}
}

Path mate(Path *p1, Path *p2, int n, double** distances){
	Path p;
	p.breedChance=0.0;
	int isAlreadyUsed[n];
	int nextpos1=1, nextpos2=1;
	
	if((p.combination = malloc(sizeof(short unsigned int)*n)) == NULL){
        return p;
	}
	
	int i,j;
	
	for(i=0; i<n; i++){
	        isAlreadyUsed[i] = 0;
			p.combination[i]=0;
	}
	
	srand ( time(NULL) );
	if(rand()%2>0){
		p.combination[0]=p1->combination[0];
		isAlreadyUsed[p1->combination[0]]=1;
	} else {
		p.combination[0]=p2->combination[0];
		isAlreadyUsed[p2->combination[0]]=1;
	}
	
	i=0; 
	while(i<n-1){
		/*Search next index from both parents*/
			
			j=0;
			while(p.combination[i] != p1->combination[j]){
				nextpos1=j+2;
				if(nextpos1>=n) nextpos1=0;
				j++;
			}
			j=0;
			while(p.combination[i] != p2->combination[j]){
				nextpos2=j+2;
				if(nextpos2>=n) nextpos2=0;
				j++;
			}
		while (isAlreadyUsed[p1->combination[nextpos1]]!=0 && isAlreadyUsed[p2->combination[nextpos2]]!=0){
			nextpos1++;
			if(nextpos1>=n) nextpos1=0;
			nextpos2++;
			if(nextpos2>=n) nextpos2=0;
		}
		if (isAlreadyUsed[p1->combination[nextpos1]]!=0){
			p.combination[i+1]=p2->combination[nextpos2];
			isAlreadyUsed[p2->combination[nextpos2]]=1;
				
				} else if(isAlreadyUsed[p2->combination[nextpos2]]!=0){
					p.combination[i+1]=p1->combination[nextpos1];
					isAlreadyUsed[p1->combination[nextpos1]]=1;
					
						} else if(distances[p.combination[i]][p1->combination[i+1]] < distances[p.combination[i]][p2->combination[i+1]]){
							p.combination[i+1]=p1->combination[nextpos1];
							isAlreadyUsed[p1->combination[nextpos1]]=1;
						
							} else { 
								p.combination[i+1]=p2->combination[nextpos2];
								isAlreadyUsed[p2->combination[nextpos2]]=1;
								
		}
		i++;
	}
	
	/*printf("After mating\n");
	for(i=0; i<n; i++){
			printf("%i %i %i %i\n",p1->combination[i], p2->combination[i], p.combination[i],isAlreadyUsed[i]);
	}*/
	
	p.combination=p.combination;
	p.fitness=path_distance(&p, distances, n);
	return p;
}

void mutate_population(Population *pop, double **distances, Config *config){
	int i;
	int n = config->numGenes;
	int mutationRate = config->mutationRate*100;
	
	srand ( time(NULL) );
	
	for(i=config->numElitism; i<pop->numPaths; i++){
		
		
		if(rand()%100 < mutationRate) {
			int a=0, b=0;
			unsigned short int temp; 
			while(a == b){
				a=rand()%n;
				b=rand()%n;
			}
			temp=pop->path[i].combination[a];
			pop->path[i].combination[a]=pop->path[i].combination[b];
			pop->path[i].combination[b]=temp;
			pop->path[i].fitness=path_distance(&pop->path[i], distances, config->numGenes);
		}
	
		
	}	
}

int path_distance(Path *path, double** distances, int n) {
	double dist = 0;
	for(int i = 0; i < n-1; i++) {
		
		dist += distances[path->combination[i]][ path->combination[i+1]];
	}
	dist += distances[path->combination[n-1]][ path->combination[0]];
	return dist;
}



/*void printPath(Path *path){
	
	
	
}*/