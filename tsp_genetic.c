#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

typedef struct City{
	char name[80];
	double latitude;
	double longitude;
	} City;
	
	
int count_lines(FILE *fptr);
void print_city(City *city);
void read_cities(FILE *fptr, City* cities[], int count);
double to_radians(double deg);
double distance(City* from, City* to);
double path_distance(City** cities, int count);

int main(void) {
	int n;
	FILE* fptr = fopen("p03.txt", "r");
	assert(fptr != NULL);
	
	
	n =	count_lines(fptr);
	printf("\nNumber of cities: %i\n",n);
	
	City* cities[n];
	read_cities(fptr, cities, n);
	
	print_city(cities[0]);
	
	printf("\nDistance between Helsinki and Espoo: %f\n", distance(cities[1],cities[4]));
	
	printf("\nDistance through default order of cities: %f\n", path_distance(cities,n));
	
	/*lueTiedosto(&x, &y, &size);

	// Tulostetaan taulukon arvot 	
	printf("%lf %lf\n", x[i], y[i]);
	for(i = 0; i < size; i++) {
  	       printf("%lf %lf\n", x[i], y[i]);
        }
	*/
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

double path_distance(City** cities, int count) {
	double dist = 0;
	for(int i = 0; i < count-1; i++) {
		dist += distance(cities[i], cities[i+1]);
	}
	dist += distance(cities[count-1], cities[0]);
	return dist;
}