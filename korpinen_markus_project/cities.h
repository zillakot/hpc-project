#ifndef CITIES_H_
#define CITIES_H_

typedef struct {
	char name[80];
	double latitude;
	double longitude;
} City;

/*typedef struct {
	int numCities;
	float **distances;
	City *city;
} Map;*/

int count_lines(FILE *fptr);
void read_cities(FILE *fptr, City** cities);
double to_radians(double deg);
void print_city(City *city);
double distance(City* from, City* to);
double** calculate_distances(double** distances, City** cities, int n);

#endif /* CITIES_H_ */