#include "TSPSelection.h"
#include "PropertiesMap.h"
#include <stdlib.h>

#include "mtrand.h"

TSPSelection::TSPSelection()
{
	std::string aux;
	PropertiesMap* properties;
	properties = PropertiesMap::instance();

	tournamentSize = 0;
	tournamentSizeToDie = 0;

	aux = properties->get("tournament");
	if(aux.length() > 0){
		char* finalPtr = 0;
		tournamentSize = strtol(aux.c_str(),&finalPtr, 10);
	}

	aux = properties->get("tournamentToDie");
	if(aux.length() > 0){
		char* finalPtr = 0;
		tournamentSizeToDie = strtol(aux.c_str(),&finalPtr, 10);
	}
}

TSPSelection::~TSPSelection()
{
}


Individual* TSPSelection::selectIndividual(Individual** population, int populationSize)
{
	Individual* ind = NULL;
	Individual* aux;
	for(int i = 0; i < tournamentSize; i++){
		aux = selectRandomIndividual(population, populationSize);
		if (ind == NULL || ind->getFitness() > aux->getFitness()){
			ind = aux;
		}
	}
	return ind;
}


Individual* TSPSelection::selectIndividualToDie(Individual** population, int populationSize)
{
	Individual* ind = NULL;
	Individual* aux;
	for(int i = 0; i < tournamentSizeToDie; i++){
		aux = selectRandomIndividual(population, populationSize);
		if (ind == NULL || ind->getFitness() < aux->getFitness()){
			ind = aux;
		}
	}
	return ind;
}


inline Individual* TSPSelection::selectRandomIndividual(Individual** population, int populationSize)
{
	MTRand_int32 irand;
	return population[irand() % populationSize];
}


