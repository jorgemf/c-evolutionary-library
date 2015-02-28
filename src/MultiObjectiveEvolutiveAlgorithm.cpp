#include "MultiObjectiveEvolutiveAlgorithm.h"
#include <cstdlib>
#include <iostream>
#include "PropertiesMap.h"

MultiObjectiveEvolutiveAlgorithm::MultiObjectiveEvolutiveAlgorithm()
{
}

MultiObjectiveEvolutiveAlgorithm::~MultiObjectiveEvolutiveAlgorithm()
{
}

void MultiObjectiveEvolutiveAlgorithm::initializeParams(){
	double elitism = 0;
	double crossover = 0;
	double mutation = 0;
	double total = 0;
	string aux;
	char *finalPtr;
	
	PropertiesMap* properties;
	properties = PropertiesMap::instance();
	
	aux = properties->get("elitismRate");
	if(aux.length() > 0){
		finalPtr = 0;
		elitism = strtod(aux.c_str(),&finalPtr);
	}
	
	aux = properties->get("crossoverRate");
	if(aux.length() > 0){
		finalPtr = 0;
		crossover = strtod(aux.c_str(),&finalPtr);
	}
	
	aux = properties->get("mutationRate");
	if(aux.length() > 0){
		finalPtr = 0;
		mutation = strtod(aux.c_str(),&finalPtr);
	}
	
	total = elitism + mutation + crossover;
	
	elitismRate = (int)(elitism * populationSize / total);
	crossoverRate = (int)(crossover * populationSize / total);
	mutationRate = (int)(mutation * populationSize / total);
}

void MultiObjectiveEvolutiveAlgorithm::run(){
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
	int numberObjectives = 0;
	aux = properties->get("objectives");
	if(aux.length() > 0){
		char* finalPtr = 0;
		numberObjectives = strtol(aux.c_str(),&finalPtr, 10);
	}
	if(populationSize > 0 && numberObjectives > 0){
	
		// creates random population
		population = new Individual*[populationSize];
		for(int i = 0; i < populationSize; i++){
			population[i] = createRandomIndividual(numberObjectives);	
		}
		// create intermediate population
		intermediatePopulation = new Individual*[populationSize];
		for(int i = 0; i < populationSize; i++){
			intermediatePopulation[i] = createEmptyIndividual(numberObjectives);	
		}
		// preloadind data if it is needed
		preload();
		// initialize the genetic algorithm params
		initializeParams();
		initialize();
		
		// evaluate firts population
		for(int i = 0; i < populationSize; i++){
			multiObjectiveEvaluation->evaluate(population[i]);
			evaluations++;
		}
		
		// set dominance
		dominanceOperator->assignDominance(population,populationSize);
		
		// set distances
		distanceFunction->assignDistances(population,populationSize);
		
		// set fitness
		for(int i = 0; i < populationSize; i++){
			int dominance = population[i]->getDominance();
			double distance = population[i]->getDistance();
			fitness = dominance + ( 1 - distance );
			population[i]->setFitness(fitness);
		}
		
		// sort the population, best individuals first
		quickSort(population, 0, populationSize-1);
		
		// iterate algorithm
		while(stopCriterion->stop(generation, population, populationSize, evaluations)){
			
			// copy elitims individuals (only best elitismRate pareto front individuals) into intermediate population
			int elitismCopied = 0;
			for(int i = 0; i < elitismRate && population[i]->getDominance() == 0; i++){
				population[i]->copy(intermediatePopulation[i]);
				for(int j = 0; j < population[i]->getNumberOfObjectives(); j++){
					intermediatePopulation[i]->setObjective(j,population[i]->getObjective(j));	
				}
				elitismCopied++;	
			}
			
			// create new crossover individuals
			Individual *indi1, *indi2, *offspring;
			for(int i = elitismCopied; i < elitismCopied + crossoverRate; i++){
				indi1 = selectionFunction->selectIndividual(population,populationSize);
				indi2 = selectionFunction->selectIndividual(population,populationSize);
				offspring = intermediatePopulation[i];
				crossoverOperator->cross(indi1,indi2,offspring);
				offspring->unsetFitness();
			}
			
			// mutated individuals
			for(int i = elitismCopied + crossoverRate; i < populationSize; i++){
				indi1 = selectionFunction->selectIndividual(population,populationSize);
				offspring = intermediatePopulation[i];
				indi1->copy(offspring);
				mutationOperator->mutate(offspring);
				offspring->unsetFitness();
			}
			
			// Exchange population and intermediate population
			Individual** aux;
			aux = population;
			population = intermediatePopulation;
			intermediatePopulation = aux;
		
			// evaluate firts population
			for(int i = elitismCopied; i < populationSize; i++){
				multiObjectiveEvaluation->evaluate(population[i]);
				evaluations++;
			}
			
			// set dominance
			dominanceOperator->assignDominance(population,populationSize);
			
			// set distances
			distanceFunction->assignDistances(population,populationSize);
			
			// set fitness
			for(int i = 0; i < populationSize; i++){
				int dominance = population[i]->getDominance();
				double distance = population[i]->getDistance();
				fitness = dominance + ( 1 - distance );
				population[i]->setFitness(fitness);
			}
		
			// sort the population, best individuals first (pareto front first individuals)
			quickSort(population, 0, populationSize-1);
		
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
		for(int i = 0; i < populationSize; i++){
			delete intermediatePopulation[i];	
		}
		delete intermediatePopulation;
	}
}


