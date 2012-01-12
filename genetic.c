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
		if((population->path[i].cityCombination = malloc((config->numGenes)*sizeof(short unsigned int))) == NULL){
	        return 1;
		}
	}
	for(i=0; i<population->numPaths; i++){

		generate_random_cityCombination(&population->path[i], config);
	    calculate_fitness(&population->path[i], config, distances);
	    population->path[i].breedChance = 0.0;


	}


	return 0;
}
	
int generate_random_cityCombination(Path *path, Config *config){
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
        path->cityCombination[i] = chosenNode;
	}

	return 0;
}
int calculate_fitness(Path *p, Config *config, double** distances){
	int fitness = 0;
	int i;
	int currentNode, nextNode;
	for(i=0; i<config->numGenes-1; i++){
		currentNode = p->cityCombination[i];
		nextNode = p->cityCombination[i+1];
		fitness += distances[currentNode][nextNode];
	}

	currentNode = p->cityCombination[config->numGenes-1];
	nextNode = p->cityCombination[0];
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
	/*Path child[3];
	int i;
	for(i=0;i<3;i++){
		child[i].fitness=0.0;
		child[i].breedingRate=0.0;
		if((child[i].cityCombination = malloc((config->numGenes)*sizeof(short unsigned int))) == NULL){
	        return 1;
		}
	}*/
	p->path[p->numPaths-1]=mate(&p->path[0], &p->path[1], n, distances);
	p->path[p->numPaths-2]=mate(&p->path[2], &p->path[3], n, distances);
	p->path[p->numPaths-3]=mate(&p->path[4], &p->path[5], n, distances);
}

Path mate(Path *p1, Path *p2, int n, double** distances){
	Path p;
	p.breedChance=0.0;
	int isAlreadyUsed[n];
	int nextpos1=1, nextpos2=1;
	
	if((p.cityCombination = malloc(sizeof(short unsigned int)*n)) == NULL){
        return p;
	}
	
	int i,j;
	
	for(i=0; i<n; i++){
	        isAlreadyUsed[i] = 0;
			p.cityCombination[i]=0;
	}
	
	srand ( time(NULL) );
	if(rand()%2>0){
		p.cityCombination[0]=p1->cityCombination[0];
		isAlreadyUsed[p1->cityCombination[0]]=1;
	} else {
		p.cityCombination[0]=p2->cityCombination[0];
		isAlreadyUsed[p2->cityCombination[0]]=1;
	}
	
	i=0; 
	while(i<n-1){
		/*Search next index from both parents*/
			
			j=0;
			while(p.cityCombination[i] != p1->cityCombination[j]){
				nextpos1=j+2;
				if(nextpos1>=n) nextpos1=0;
				j++;
			}
			j=0;
			while(p.cityCombination[i] != p2->cityCombination[j]){
				nextpos2=j+2;
				if(nextpos2>=n) nextpos2=0;
				j++;
			}
		while (isAlreadyUsed[p1->cityCombination[nextpos1]]!=0 && isAlreadyUsed[p2->cityCombination[nextpos2]]!=0){
			nextpos1++;
			if(nextpos1>=n) nextpos1=0;
			nextpos2++;
			if(nextpos2>=n) nextpos2=0;
		}
		if (isAlreadyUsed[p1->cityCombination[nextpos1]]!=0){
			p.cityCombination[i+1]=p2->cityCombination[nextpos2];
			isAlreadyUsed[p2->cityCombination[nextpos2]]=1;
				
				} else if(isAlreadyUsed[p2->cityCombination[nextpos2]]!=0){
					p.cityCombination[i+1]=p1->cityCombination[nextpos1];
					isAlreadyUsed[p1->cityCombination[nextpos1]]=1;
					
						} else if(distances[p.cityCombination[i]][p1->cityCombination[i+1]] < distances[p.cityCombination[i]][p2->cityCombination[i+1]]){
							p.cityCombination[i+1]=p1->cityCombination[nextpos1];
							isAlreadyUsed[p1->cityCombination[nextpos1]]=1;
						
							} else { 
								p.cityCombination[i+1]=p2->cityCombination[nextpos2];
								isAlreadyUsed[p2->cityCombination[nextpos2]]=1;
								
		}
		i++;
	}
	
	/*printf("After mating\n");
	for(i=0; i<n; i++){
			printf("%i %i %i %i\n",p1->cityCombination[i], p2->cityCombination[i], p.cityCombination[i],isAlreadyUsed[i]);
	}*/
	
	p.cityCombination=p.cityCombination;
	p.fitness=path_distance(&p, distances, n);
	return p;
}

void breed_population(Population *population, Config *config){
	BreedersList brl;
	brl.numBreeders = 0;
	brl.breeders = NULL;
	
	calculate_breed_chance(population);
	select_breeders(&brl, population, config->maxBreeding);
	
	
}

void calculate_breed_chance(Population *pop){
        int i;
        //int minDist = pop->path[0].fitness;
        float rate = 0.0;

        for(i=1; i<pop->numPaths; i++){
			rate = pow(2,i);
                pop->path[i].breedChance = 1 / rate;
        }
}

int select_breeders(BreedersList *brl, Population *pop, int maxBreeding){
        Path *parent1 = NULL, *parent2 = NULL;
        char *alreadyBreed = NULL;
        int numBreeding = maxBreeding;
        int i, j;

        if((alreadyBreed = (char*)malloc(sizeof(char) * pop->numPaths)) == NULL){
                return 1;
        }
        memset(alreadyBreed, 0, sizeof(char) * pop->numPaths);

        for(i=0; i<pop->numPaths-1; i++){

                if(numBreeding == 0)
                        break;

                if(alreadyBreed[i] == 1)
                        continue;

                if(rand()%100 > (pop->path[i].breedChance * 100))
                        continue;

                alreadyBreed[i] = 1;
                parent1 = &pop->path[i];

                brl->numBreeders += 1;
                if((brl->breeders = (Breeders*)realloc(brl->breeders, sizeof(Breeders)*brl->numBreeders)) == NULL){
                        free(alreadyBreed);
                        return 1;
                }
                memcpy(&brl->breeders[brl->numBreeders-1].parent1, parent1, sizeof(Path));

                for(j=i+1; j<pop->numPaths; j++){
                        if(alreadyBreed[j] == 1)
                                continue;

                        if(rand()%100 > (pop->path[j].breedChance * 100))
                                continue;

                        parent2 = &pop->path[j];
                        alreadyBreed[j] = 1;
                        break;
                }

                if(parent2 == NULL){
                        j = i+1;
                        while(alreadyBreed[j] != 0){
                                if(j+1 < pop->numPaths)
                                        j++;
                                else
                                        j=0;

                        }
                        parent2 = &pop->path[j];
                        alreadyBreed[j] = 1;
                }

                memcpy(&brl->breeders[brl->numBreeders-1].parent2, parent2, sizeof(Path));

                memset(&brl->breeders[brl->numBreeders-1].children, pop->numPaths-1, sizeof(Path));

                numBreeding--;
        }

        free(alreadyBreed);
		
		//printf("parent1 fitness:%i\n",parent1->fitness);
		//printf("parent2 fitness:%i\n",parent2->fitness);
		
		
        return 0;
}

int path_distance(Path *path, double** distances, int n) {
	double dist = 0;
	for(int i = 0; i < n-1; i++) {
		
		dist += distances[path->cityCombination[i]][ path->cityCombination[i+1]];
	}
	dist += distances[path->cityCombination[n-1]][ path->cityCombination[0]];
	return dist;
}

void mutate_population(Population *pop, Config *config){
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
			temp=pop->path[i].cityCombination[a];
			pop->path[i].cityCombination[a]=pop->path[i].cityCombination[b];
			pop->path[i].cityCombination[b]=temp;
		}
	
		
	}	
}