#include "AlgorithmTSPGA2.h"
#include "TSPCrossover2.h"
#include "TSPFitness.h"
#include "TSPMutation2.h"
#include "TSPSelection.h"
#include "TSPStopCriterion.h"

AlgorithmTSPGA2::AlgorithmTSPGA2()
{
}

AlgorithmTSPGA2::~AlgorithmTSPGA2()
{
}

void AlgorithmTSPGA2::initialize()
{
	crossoverOperator = new TSPCrossover2();
	fitnessFunction = new TSPFitness(this->citiesDistances, this->citiesCoordinates, this->dataType);
	mutationOperator = new TSPMutation2();
	selectionFunction = new TSPSelection();
	stopCriterion = new TSPStopCriterion();
}

	
void AlgorithmTSPGA2::endGeneration()
{
	algorithmStats(false,population,populationSize,generation,evaluations);
}

	
void AlgorithmTSPGA2::preload()
{
}

	
void AlgorithmTSPGA2::endAlgorithm()
{
	algorithmStats(true,population,populationSize,generation,evaluations);
	
	delete crossoverOperator;
	delete fitnessFunction;
	delete mutationOperator;
	delete selectionFunction;
	delete stopCriterion;
}

	
Individual* AlgorithmTSPGA2::createRandomIndividual(int objectives)
{
	return generateRandomIndividualDoubles();
}


Individual* AlgorithmTSPGA2::createEmptyIndividual(int objectives)
{
	return generateRandomIndividualDoubles();
}


