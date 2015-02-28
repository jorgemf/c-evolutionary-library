#include "AlgorithmTSPGA.h"
#include "TSPCrossover.h"
#include "TSPFitness.h"
#include "TSPMutation.h"
#include "TSPSelection.h"
#include "TSPStopCriterion.h"

AlgorithmTSPGA::AlgorithmTSPGA()
{
}

AlgorithmTSPGA::~AlgorithmTSPGA()
{
}

void AlgorithmTSPGA::initialize()
{
	crossoverOperator = new TSPCrossover();
	fitnessFunction = new TSPFitness(this->citiesDistances, this->citiesCoordinates, this->dataType);
	mutationOperator = new TSPMutation();
	selectionFunction = new TSPSelection();
	stopCriterion = new TSPStopCriterion();
}

	
void AlgorithmTSPGA::endGeneration()
{
	algorithmStats(false,population,populationSize,generation,evaluations);
}

	
void AlgorithmTSPGA::preload()
{
}

	
void AlgorithmTSPGA::endAlgorithm()
{
	algorithmStats(true,population,populationSize,generation,evaluations);
	
	delete crossoverOperator;
	delete fitnessFunction;
	delete mutationOperator;
	delete selectionFunction;
	delete stopCriterion;
}

	
Individual* AlgorithmTSPGA::createRandomIndividual(int objectives)
{
	return generateRandomIndividualInts();
}


Individual* AlgorithmTSPGA::createEmptyIndividual(int objectives)
{
	return generateRandomIndividualInts();
}


