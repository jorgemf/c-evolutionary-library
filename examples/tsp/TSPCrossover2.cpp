#include "TSPCrossover2.h"
#include "AlgorithmTSP.h"
#include "TSPIndividual2.h"
#include "mtrand.h"

#include <iostream>

TSPCrossover2::TSPCrossover2()
{
}


TSPCrossover2::~TSPCrossover2()
{
}


void TSPCrossover2::cross(Individual* c1, Individual* c2, Individual* offspring)
{
	MTRand rand;
	MTRand_int32 irand;
	
	double* cities1 = ((TSPIndividual2*)c1)->getDoubleRoute();
	double* cities2 = ((TSPIndividual2*)c2)->getDoubleRoute();
	double* citiesOffspring = ((TSPIndividual2*)offspring)->getDoubleRoute();
	int numberOfCities = ((TSPIndividual2*)offspring)->getNumberCities();
	
	
	//select initial and end point
	int point1 = irand() % numberOfCities;
	int point2 = point1 + 1 + irand() % (numberOfCities - point1);
	
	// copy parentsinto offspring
	for(int i = 0; i < point1; i++){
		citiesOffspring[i] = cities1[i];
	}
	for(int i = point1; i < point2; i++){
		citiesOffspring[i] = cities2[i];
	}
	for(int i = point2; i < numberOfCities; i++){
		citiesOffspring[i] = cities1[i];
	}
	
	// creck for no repetaed numbers (first cities not repeated)
	for(int i = point1; i < numberOfCities; i++){
		int j = i;
		bool repeated = false;
		while(j >= 0 && !repeated){
			repeated = citiesOffspring[i] == citiesOffspring[j];
			j--;
		}
		if(repeated){
			citiesOffspring[i] = AlgorithmTSP::generateRandomNumberNotInArray(citiesOffspring, i);
		}
	}
}
