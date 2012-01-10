#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <time.h>


typedef struct {
	int populationSize;
	double mutationRate;
	int numGenerations;
	int numElitism;
	int mutationSize;
	int maxBreeding;
	int numCities;
} Config;

typedef struct {
	char name[80];
	double latitude;
	double longitude;
} City;
/*
typedef struct {
	int numCities;
	float **distances;
	City *city
} CityList;
*/	
typedef struct {
	int pathDistance;
	float breedChance;
	short unsigned int *cityCombination;
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
	
int count_lines(FILE *fptr);
void print_city(City *city);
void read_cities(FILE *fptr, City** cities, int count);
double to_radians(double deg);
double distance(City* from, City* to);
double path_distance(City** cities, int count);
double** calculate_distances(double** distances, City** cities, int n);

int generate_random_population(Population *population, City** cities, double** distances, Config *config);
int generate_random_cityCombination(Path *path, Config *config);
int calculatepath_distance(Path *path, City **cities, Config *config, double** distances);
int compare_population(const void * a, const void * b);
int mutatePopulation(Population *population, City **cities, Config *config);
void printPath(Path *path, unsigned long int numGenerations);
void print_population(Population *population);
void breed_population(Population *population, Config *config);
void simple_breed_population(Population *population, int n, double** distances);
void calculate_breed_chance(Population *population);
int select_breeders(BreedersList *brl, Population *population, int maxBreeding);
Path mate(Path *p1, Path *p2, int n, double** distances);

int main(void) {
	int n,i,j;
	FILE* fptr = fopen("input.in", "r");
	assert(fptr != NULL);
	
	n =	count_lines(fptr);
	printf("\nNumber of cities: %i\n",n);
	
	/*Init cities*/
	City* cities[n];
	read_cities(fptr, cities, n);
	printf("Init cities succesful... \n");
	
	/*Init distances*/
	double** distances;
	distances=calculate_distances(distances,cities, n);
	printf("Init distances succesful... \n");
	
	/*Init GA config*/
	Config config;
	config.populationSize=100;
	config.mutationRate=0.1;
	config.numGenerations=2;
	config.numElitism=1;
	config.mutationSize=1;
	config.maxBreeding=10;
	config.numCities=n;
	printf("Init GA config succesful... \n");
	
	/*Init random seed*/
	srand ( time(NULL) );
	printf("Init random seed succesful... \n");
	
	/*Init population*/
    Population population;
	generate_random_population(&population, cities, distances, &config);
	printf("Init population succesful... \n");
	
	/*Init mostFit*/
    Path mostFit;
	mostFit.cityCombination=malloc(n*sizeof(short unsigned int));
	mostFit.pathDistance = 999999;
    unsigned long int numGenerations = 0;
	printf("Init mostFit succesful... \n");
	
	/*Start evolution*/
	while(numGenerations < config.numGenerations){
		numGenerations++;
		
		/*Sort population*/
		qsort(population.path, population.numPaths, sizeof(Path), compare_population);
		
		/*Breed population*/
		/*Only 6 shortest pathDistance gets to breed*/
		/*Breeding can be also easily improved*/
		
		simple_breed_population(&population, config.numCities, distances);
		
		/*breed_population(&population, &config);*/
		
	}
	
	print_population(&population);
	
return 0;
}

void print_city(City *city){
	
	printf("\n%s (%lf %lf)\n",city->name,city->latitude,city->longitude);
	
}

int count_lines(FILE *fptr){
	int n=0;
	char str[81];
	rewind(fptr);
		while(fgets(str, 80, fptr) != NULL) {
		if((str[0] != '#') && (str[0] != '\n')) {
			n++;
		}
	}
	return n;
}

void read_cities(FILE *fptr, City* cities[], int count){
	rewind(fptr);
	int n=0;
	char str[81];
	
	while(fgets(str, 80, fptr) != NULL) {
		if((str[0] != '#') && (str[0] != '\n')) {
			cities[n] = malloc(sizeof(City));
			sscanf(str, "%s %lf %lf", cities[n]->name, &cities[n]->latitude, &cities[n]->longitude);
			n++;
		}
	}
	return;	
}

double to_radians(double deg) {
	return deg*(3.14159/180);
}

double distance(City* from, City* to) {
	double f_lat_rad = to_radians(from->latitude);
	double t_lat_rad = to_radians(to->latitude);
	double f_lon_rad = to_radians(from->longitude);
	double t_lon_rad = to_radians(to->longitude);

	double diff_lat = fabs(t_lat_rad - f_lat_rad);
	double diff_lon = fabs(t_lon_rad - f_lon_rad);

	double a = sin(diff_lat/2)*sin(diff_lat/2) + (cos(f_lat_rad)*cos(t_lat_rad)*sin(diff_lon/2)*sin(diff_lon/2));

	double c = 2*atan2(sqrt(a), sqrt(1-a));

	return c*6370;
}

double** calculate_distances(double** distances, City** cities, int n){
	int i,j;
	if((distances = malloc(n*sizeof(double*))) == NULL){
            return NULL;
    }
	for(i=0; i<n; ++i){
		if((distances[i]=malloc(n*sizeof(double))) == NULL){
			return NULL;
		}
	}
	for(i=0; i<n ; i++){
		for(j=0; j<n ; j++){
			if (i==j) distances[i][j]=0;
	    	if (i!=j) {
				distances[i][j]=distance(cities[i],cities[j]);	
			}
		}
	}
	return distances;
}

double path_distance(City** cities, int count) {
	double dist = 0;
	for(int i = 0; i < count-1; i++) {
		dist += distance(cities[i], cities[i+1]);
	}
	dist += distance(cities[count-1], cities[0]);
	return dist;
}

int generate_random_population(Population *population, City** cities, double** distances, Config *config){
	int i;

	population->numPaths = config->populationSize;

	if((population->path = (Path*)malloc(sizeof(Path) * population->numPaths)) == NULL){
	    return 1;
	}
	for(i=0; i<population->numPaths; i++){
		if((population->path[i].cityCombination = malloc((config->numCities)*sizeof(short unsigned int))) == NULL){
	        return 1;
		}
	}
	for(i=0; i<population->numPaths; i++){

		generate_random_cityCombination(&population->path[i], config);
	    calculatepath_distance(&population->path[i], cities, config, distances);
	    population->path[i].breedChance = 0.0;


	}


	return 0;
}
	
int generate_random_cityCombination(Path *path, Config *config){
	int isAlreadyUsed[config->numCities];
	short unsigned int chosenNode = 0;
	int i;

	for(i=0; i<config->numCities; i++){
	        isAlreadyUsed[i] = 0;
	}
	for(i=0; i<config->numCities; i++){

        // choose random node of the map
        //chosenNode = lrand48() % config->numCities;
        chosenNode = rand() % config->numCities;
        //if this node was chosen already
        if(isAlreadyUsed[chosenNode] != 0){

	        //choose the next one of the list
	        while(isAlreadyUsed[chosenNode] != 0){
	            if(chosenNode + 1 < config->numCities){
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
int calculatepath_distance(Path *p, City** cities, Config *config, double** distances){
	int pathDistance = 0;
	int i;
	int currentNode, nextNode;
	for(i=0; i<config->numCities-1; i++){
		currentNode = p->cityCombination[i];
		nextNode = p->cityCombination[i+1];
		pathDistance += distances[currentNode][nextNode];
	}

	currentNode = p->cityCombination[config->numCities-1];
	nextNode = p->cityCombination[0];
	pathDistance += distances[currentNode][nextNode];

	p->pathDistance = pathDistance;
	return 0;
}

/* qsort function*/
int compare_population(const void * a, const void * b){
	Path *a1 = (Path*)a;
	Path *b1 = (Path*)b;

	return (a1->pathDistance - b1->pathDistance);
}

void print_population(Population *population){
	int i;
	for(i=0;i<population->numPaths;i++){
		printf("n:%i, pathDistance:%i\n",i,population->path[i].pathDistance);
	}
	
}

void simple_breed_population(Population *p, int n, double** distances){
	/*Path child[3];
	int i;
	for(i=0;i<3;i++){
		child[i].pathDistance=0.0;
		child[i].breedingRate=0.0;
		if((child[i].cityCombination = malloc((config->numCities)*sizeof(short unsigned int))) == NULL){
	        return 1;
		}
	}*/
	
	p->path[p->numPaths-1]=mate(&p->path[0], &p->path[1], n, distances);
	p->path[p->numPaths-2]=mate(&p->path[2], &p->path[3], n, distances);
	p->path[p->numPaths-3]=mate(&p->path[4], &p->path[5], n, distances);
}

Path mate(Path *p1, Path *p2, int n, double** distances){
	Path p;
	p.pathDistance=0.0;
	p.breedChance=0.0;
	int isAlreadyUsed[n];
	int nextpos1, nextpos2;
	
	int i=0,j=0,k=0;
	
	short unsigned int cityArray[n];
	
	for(i=0; i<n; i++){
	        isAlreadyUsed[i] = 0;
	}
	
	if(rand()%2>0) cityArray[0]=p1->cityCombination[0];
	if(rand()%2<1) cityArray[0]=p2->cityCombination[0];
	isAlreadyUsed[0]=1;
	
	while(i<n-1){
		/*Search next index from both parents*/
		for(j=0;j<n;j++){
			if(cityArray[i]==p1->cityCombination[j]) nextpos1=j+1;
			if(cityArray[i]==p2->cityCombination[j]) nextpos2=j+1;
			
			/*Boundary*/
			if(nextpos1>=n) nextpos1==0;
			if(nextpos2>=n) nextpos2==0;
		}
		while (isAlreadyUsed[nextpos1]!=0 && isAlreadyUsed[nextpos2]!=0){
			nextpos1++;
			nextpos2++;
		}
		if (isAlreadyUsed[nextpos1]!=0){
			cityArray[i+1]=p2->cityCombination[nextpos2];
			isAlreadyUsed[nextpos2];
			
			} else if (isAlreadyUsed[nextpos2]==0){
				cityArray[i+1]=p1->cityCombination[nextpos1];
				isAlreadyUsed[nextpos1];
				
				} else if(distances[cityArray[i]][p1->cityCombination[i+1]] < distances[cityArray[i]][p2->cityCombination[i+1]]){
					cityArray[i+1]=p1->cityCombination[nextpos1];
					isAlreadyUsed[nextpos1];
					
					} else { 
						cityArray[i+1]=p2->cityCombination[nextpos2];
						isAlreadyUsed[nextpos2];
		}
		i++;
	}
	
	p.cityCombination=cityArray;
	
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
        int minDist = pop->path[0].pathDistance;
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
		
		//printf("parent1 pathDistance:%i\n",parent1->pathDistance);
		//printf("parent2 pathDistance:%i\n",parent2->pathDistance);
		
		
        return 0;
}

