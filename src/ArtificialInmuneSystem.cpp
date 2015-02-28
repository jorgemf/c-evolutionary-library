#include <cstdlib>
#include "ArtificialInmuneSystem.h"
#include "PropertiesMap.h"

ArtificialInmuneSystem::ArtificialInmuneSystem()
{
}

ArtificialInmuneSystem::~ArtificialInmuneSystem()
{
}

void ArtificialInmuneSystem::initializeParams(){
	string aux;
	
	PropertiesMap* properties;
	properties = PropertiesMap::instance();
}


void ArtificialInmuneSystem::run(){
	string aux;
	PropertiesMap* properties;
	properties = PropertiesMap::instance();
	double fitness;
	
	populationSize = 0;
	generation = 0;
	evaluations = 0;
	aux = properties->get("population");
	if(aux.length() > 0){
		char* finalPtr = 0;
		populationSize = strtol(aux.c_str(),&finalPtr, 10);
	}
	if(populationSize > 0){
	
		// creates random population
		population = new Individual*[populationSize];
		for(int i = 0; i < populationSize; i++){
			population[i] = createRandomIndividual();	
		}
		// preloadind data if it is needed
		preload();
		// initialize the genetic algorithm params
		initializeParams();
		initialize();
		
		// evaluate firts population
		for(int i = 0; i < populationSize; i++){
			if(!population[i]->isSetFitness()){
				fitness = fitnessFunction->evaluate(population[i]);
				population[i]->setFitness(fitness);
				evaluations++;
			}
		}

		// calculate maximal, minimal fitness
		double maxFit = population[0]->getFitness();
		double minFit = population[0]->getFitness();
		
		for(int i = 0; i < populationSize; i++){
			fitness = population[i]->getFitness();
			if(maxFit < fitness){
				maxFit = fitness;	
			}else if(minFit > fitness){
				minFit = fitness;	
			}
		}
		
		// iterate algorithm
		while(stopCriterion->stop(generation, population, populationSize, evaluations)){
			
			Individual *individual, *offspring;
			// select individual to clone
			individual = selectionFunction->selectIndividual(population,populationSize);
			// select individual to die
			offspring = selectionFunction->selectIndividualToDie(population,populationSize);
			// copy the chromosome if they are differents
			if(individual != offspring){
				individual->copy(offspring);	
			}
			fitness = individual->getFitness();
			// mutate as much as fitness points
			// 1 - because 0 is the best fitness
			double mutation = (fitness - minFit) / (maxFit - minFit);
			mutationOperator->mutateRange(offspring,mutation);

			// recalcule the fitness				
			fitness = fitnessFunction->evaluate(offspring);
			offspring->setFitness(fitness);
			evaluations++;
			
			if(maxFit < fitness){
				maxFit = fitness;	
			}else if(minFit > fitness){
				minFit = fitness;	
			}else{
				maxFit = fitness;
				fitness = fitness;
				for(int i = 0; i < populationSize; i++){
					fitness = population[i]->getFitness();
					if(maxFit < fitness){
						maxFit = fitness;	
					}else if(minFit > fitness){
						minFit = fitness;	
					}
				}
			}
		
			// end of this generation
			generation++;
			endGeneration();
		}
		
	}
	endAlgorithm();
	
	//free memory
	if(populationSize > 0){
		for(int i = 0; i < populationSize; i++){
			delete population[i];	
		}
		delete population;
	}
}
	
	
