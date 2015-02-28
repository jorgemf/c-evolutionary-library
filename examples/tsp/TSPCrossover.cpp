#include "TSPCrossover.h"
#include "TSPIndividual.h"
#include "mtrand.h"

#include <iostream>

TSPCrossover::TSPCrossover()
{
	
}

TSPCrossover::~TSPCrossover()
{
	
}


void TSPCrossover::cross(Individual* c1, Individual* c2, Individual* offspring)
{
	MTRand_int32 irand;
	
	int* cities1 = ((TSPIndividual*)c1)->getCitiesRoute();
	int* cities2 = ((TSPIndividual*)c2)->getCitiesRoute();
	int* citiesOffspring = ((TSPIndividual*)offspring)->getCitiesRoute();
	int numberOfCities = ((TSPIndividual*)offspring)->getNumberCities();
	
	//select initial and end point
	int point1 = irand() % numberOfCities;
	int point2 = point1 + 1 + irand() % (numberOfCities - point1);
	
	// copy parent 1 array into offspring
	for(int i = 0; i < numberOfCities; i++){
		citiesOffspring[i] = cities1[i];	
	}
	
	// delete in offspring the cities to be copied
	for(int i = point1; i < point2; i++){
		int j = 0;
		while(cities2[i] != citiesOffspring[j]){
			j++;	
		}
		citiesOffspring[j] = 0;
	}
	
	// move cities before point1 to fill gaps
	for(int i = 0; i < point1; i++){
		if(	citiesOffspring[i] == 0 ){
			int j = i+1;
			while( citiesOffspring[i] == 0 ){
				int aux = citiesOffspring[i];
				citiesOffspring[i] = citiesOffspring[j];
				citiesOffspring[j] = aux;
				j++;
			}
		}
	}
	
	// move cities after point2 to fill gaps
	for(int i = numberOfCities-1; i >= point2; i--){
		if(	citiesOffspring[i] == 0 ){
			int j = i-1;
			while( citiesOffspring[i] == 0 ){
				int aux = citiesOffspring[i];
				citiesOffspring[i] = citiesOffspring[j];
				citiesOffspring[j] = aux;
				j--;
			}
		}
	}
	
	// copy the cities of c2 in offspring
	for(int i = point1; i < point2; i++){
		citiesOffspring[i] = cities2[i];
	}
}
