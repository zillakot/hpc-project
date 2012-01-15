#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "cities.h"

void print_city(City *city){
	
	printf("%s (%lf %lf)\n",city->name,city->latitude,city->longitude);
	
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

void read_cities(FILE *fptr, City* cities[]){
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
	return deg*(3.14159/180.0);
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