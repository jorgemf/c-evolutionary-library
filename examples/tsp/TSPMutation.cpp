#include "TSPMutation.h"
#include <iostream>
#include <stdlib.h>

#include "PropertiesMap.h"

#include "mtrand.h"


TSPMutation::TSPMutation()
{
	std::string aux;
	PropertiesMap* properties;
	properties = PropertiesMap::instance();

	minMutations = 0;
	maxMutations = 0;

	aux = properties->get("mutationMinMutations");
	if(aux.length() > 0){
		char* finalPtr = 0;
		minMutations = strtol(aux.c_str(),&finalPtr, 10);
	}

	aux = properties->get("mutationMaxMutations");
	if(aux.length() > 0){
		char* finalPtr = 0;
		maxMutations = strtol(aux.c_str(),&finalPtr, 10);
	}

	this->numberOfCities = 0;
}


TSPMutation::~TSPMutation()
{
	if(this->numberOfCities > 0){
		delete probabilities;
	}
}


void TSPMutation::mutate(Individual* individual)
{
	MTRand_int32 irand;
	TSPIndividual* individualTSP = (TSPIndividual*)individual;
	switch(irand()%2){
		case 0:
			exchangeRegions(individualTSP);
			break;
		case 1:
			reverseRegion(individualTSP);
			break;
	}
}


void TSPMutation::mutateRange(Individual* individual, double mutation)
{
	int mutations = (int)(mutation*(maxMutations-minMutations)+minMutations);
	for(int i = 0; i < mutations; i++){
		mutate(individual);
	}
}


int TSPMutation::regionSize(TSPIndividual* individual)
{
	int maxSize = this->numberOfCities*5/12;
	if(this->numberOfCities != individual->getNumberCities()){

		// recalculate probabilities

		if(this->numberOfCities > 0){
			delete probabilities;
		}
		this->numberOfCities = individual->getNumberCities();
		maxSize = this->numberOfCities*5/12;
		probabilities = new double[maxSize];

		// P(X) = ( 1 / X ) / sum{from i=1 to N}{ 1/i }
		// P(X) = 1/X
		double sum = 0;
		for(int i = 0; i < maxSize; i++){
			probabilities[i] = 1.0 / (i+1);
			sum += probabilities[i];
		}
		// P(X) = ( 1 / X ) / sum{from i=1 to N}{ 1/i }
		for(int i = 0; i < maxSize; i++){
			probabilities[i] /= sum;
		}

		// sum for rulete tournament (similar)
		for(int i = 1; i < maxSize; i++){
			probabilities[i] = probabilities[i] + probabilities[i-1];
		}
	}

	// random
	MTRand rand;
	double value = rand();
	int size = 1;
	while(size < maxSize && probabilities[size-1] < value){
		size++;
	}
	// returns the region size
	return size;
}

void TSPMutation::exchangeRegions(TSPIndividual* individual)
{
	MTRand_int32 irand;

	int size = regionSize(individual);
	int numberOfCities = individual->getNumberCities();
	int* cities = individual->getCitiesRoute();

	// select first point of the first region
	int point1 = irand() % (numberOfCities - 2 * size);
	// select first point of the second region
	int point2 = point1 + size + irand() % (numberOfCities - 2*size - point1);

	if( point1 + size*2 <= numberOfCities && point2 + size <= numberOfCities && point1 + size <= point2){
		for(int i = 0; i < size; i++){
			int aux = cities[i+point1];
			cities[i+point1] = cities[i+point2];
			cities[i+point2] = aux;
		}
	}else{
		std::cerr << "Error in region exchanges" << std::endl;
	}
}


void TSPMutation::reverseRegion(TSPIndividual* individual)
{
	MTRand_int32 irand;

	int numberOfCities = individual->getNumberCities();
	int size = 1 + irand() % (numberOfCities-1);
	int* cities = individual->getCitiesRoute();

	// select first point of the region
	int point = irand() % (numberOfCities - size);
	for(int i = 0; i < size/2; i++){
		int aux = cities[i+point];
		cities[i+point] = cities[size-i-1+point];
		cities[size-i-1+point] = aux;
	}
}

