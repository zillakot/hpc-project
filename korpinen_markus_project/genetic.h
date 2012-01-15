#ifndef GENETIC_H_
#define GENETIC_H_

#include "cities.h"

typedef struct {
	int fitness;
	float breedChance;
	short unsigned int *combination;
} Path; 
	
typedef struct {
	int numPaths;
	Path *path;
} Population;

typedef struct {
	Path parent1;
	Path parent2;
	Path children;
} Breeders;

typedef struct {
	int numBreeders;
	Breeders *breeders;
} BreedersList;

typedef struct {
	int populationSize;
	double mutationRate;
	int numGenerations;
	int numElitism;
	int mutationSize;
	int maxBreeding;
	int numGenes;
} Config;

int generate_random_population(Population *population,  double** distances, Config *config);
int generate_random_combination(Path *path, Config *config);
int calculate_fitness(Path *path, Config *config, double** distances);
int compare_population(const void * a, const void * b);
void printPath(Path *path);
void print_population(Population *population);
void breed_population(Population *population, Config *config);
void simple_breed_population(Population *population, int n, double** distances);
void calculate_breed_chance(Population *population);
int select_breeders(BreedersList *brl, Population *population, int maxBreeding);
Path mate(Path *p1, Path *p2, int n, double** distances);
int path_distance(Path *path, double** distances, int n);
void mutate_population(Population *population, double **distances, Config *config);

#endif /* GENETIC_H_ */