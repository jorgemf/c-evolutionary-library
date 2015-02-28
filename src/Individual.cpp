#include "Individual.h"

Individual::Individual(int objectives)
{
	fitness = -1;
	fitnessSetted = false;
	distance = 0;
	dominance = 0;
	numberOfObjectives = objectives;
	if(numberOfObjectives > 0){
		this->objectives = new double[numberOfObjectives];
	}
}

Individual::~Individual()
{
	if(numberOfObjectives > 0){
		delete objectives;
	}
}


void Individual::copy(Individual* individual)
{
	individual->fitness = fitness;
	individual->fitnessSetted = fitnessSetted;
	individual->distance = distance;
	individual->dominance = dominance;
	if(individual->numberOfObjectives != numberOfObjectives ){
		delete individual->objectives;
		if(numberOfObjectives > 0){
			individual->objectives = new double[numberOfObjectives];
		}
		individual->numberOfObjectives = numberOfObjectives;
	}
	for(int i = 0; i < numberOfObjectives; i++){
		individual->objectives[i] = objectives[i];	
	}
}


void Individual::setObjective(int number, double objective)
{
	if(number >= 0 && number < this->numberOfObjectives){
		this->objectives[number] = objective;	
	}
}
	
	
double Individual::getObjective(int number)
{
	double obj = 0;
	if(number >= 0 && number < this->numberOfObjectives){
		obj = this->objectives[number];	
	}
	return obj;
}

