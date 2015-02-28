#include "TSPMutation2.h"
#include <iostream>

#include "AlgorithmTSP.h"
#include "mtrand.h"



TSPMutation2::TSPMutation2()
{
}


TSPMutation2::~TSPMutation2()
{
}


void TSPMutation2::mutate(Individual* individual)
{
	MTRand_int32 irand;
	TSPIndividual2* individualTSP = (TSPIndividual2*)individual;
	switch(irand()%3){
		case 0:
			exchangeRegions(individualTSP);
			break;
		case 1:
			reverseRegion(individualTSP);
			break;	
		case 3:
			mutateNumber(individualTSP);
			break;	
	}
}
	
void TSPMutation2::exchangeRegions(TSPIndividual2* individual)
{
	MTRand_int32 irand;
	
	int size = regionSize(individual);
	int numberOfCities = individual->getNumberCities();
	double* cities = individual->getDoubleRoute();
	
	// select first point of the first region
	int point1 = irand() % (numberOfCities - 2 * size);
	// select first point of the second region
	int point2 = point1 + size + irand() % (numberOfCities - 2*size - point1);
	
	if( point1 + size*2 <= numberOfCities && point2 + size <= numberOfCities && point1 + size <= point2){
		for(int i = 0; i < size; i++){
			double aux = cities[i+point1];	
			cities[i+point1] = cities[i+point2];
			cities[i+point2] = aux;
		}
	}else{
		std::cerr << "Error in region exchanges" << std::endl;	
	}
}

	
void TSPMutation2::reverseRegion(TSPIndividual2* individual)
{
	MTRand_int32 irand;
	
	int numberOfCities = individual->getNumberCities();
	int size = 1 + irand() % (numberOfCities-1);
	double* cities = individual->getDoubleRoute();
	
	// select first point of the region
	int point = irand() % (numberOfCities - size);
	for(int i = 0; i < size/2; i++){
		double aux = cities[i+point];
		cities[i+point] = cities[size-i-1+point];
		cities[size-i-1+point] = aux;
	}
}

	
void TSPMutation2::mutateNumber(TSPIndividual2* individual)
{
	MTRand_int32 irand;
	
	int numberOfCities = individual->getNumberCities();
	int point = irand() % numberOfCities;
	double* cities = individual->getDoubleRoute();
	
	cities[point] = AlgorithmTSP::generateRandomNumberNotInArray(cities, numberOfCities);	
}


