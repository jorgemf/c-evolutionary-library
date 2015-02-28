#include <cstdlib>
#include "GeneticAlgorithm.h"
#include "PropertiesMap.h"

GeneticAlgorithm::GeneticAlgorithm()
{
}

GeneticAlgorithm::~GeneticAlgorithm()
{
}

void GeneticAlgorithm::initializeParams(){
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

void GeneticAlgorithm::run(){
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
		// create intermediate population
		intermediatePopulation = new Individual*[populationSize];
		for(int i = 0; i < populationSize; i++){
			intermediatePopulation[i] = createEmptyIndividual();	
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
		
		// iterate algorithm
		while(stopCriterion->stop(generation, population, populationSize, evaluations)){
			
			// sort the population, best individuals first
			quickSort(population, 0, populationSize-1);
			
			// copy elitims individuals into intermediate population
			for(int i = 0; i < elitismRate; i++){
				population[i]->copy(intermediatePopulation[i]);	
				intermediatePopulation[i]->setFitness(population[i]->getFitness());			
				
			}
			
			// create new crossover individuals
			Individual *indi1, *indi2, *offspring;
			for(int i = elitismRate; i < elitismRate + crossoverRate; i++){
				indi1 = selectionFunction->selectIndividual(population,populationSize);
				indi2 = selectionFunction->selectIndividual(population,populationSize);
				offspring = intermediatePopulation[i];
				crossoverOperator->cross(indi1,indi2,offspring);
				offspring->unsetFitness();
			}
			
			// mutated individuals
			for(int i = elitismRate + crossoverRate; i < populationSize; i++){
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
			
			// evaluate population
			for(int i = 0; i < populationSize; i++){
				if(!population[i]->isSetFitness()){
					fitness = fitnessFunction->evaluate(population[i]);
					population[i]->setFitness(fitness);
					evaluations++;
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
		for(int i = 0; i < populationSize; i++){
			delete intermediatePopulation[i];	
		}
		delete intermediatePopulation;
	}
}

