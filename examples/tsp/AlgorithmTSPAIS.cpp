#include "AlgorithmTSPAIS.h"
#include "TSPFitness.h"
#include "TSPMutation.h"
#include "TSPSelection.h"
#include "TSPStopCriterion.h"

AlgorithmTSPAIS::AlgorithmTSPAIS()
{
}

AlgorithmTSPAIS::~AlgorithmTSPAIS()
{
}

void AlgorithmTSPAIS::initialize()
{
	fitnessFunction = new TSPFitness(this->citiesDistances, this->citiesCoordinates, this->dataType);
	mutationOperator = new TSPMutation();
	selectionFunction = new TSPSelection();
	stopCriterion = new TSPStopCriterion();
}

	
void AlgorithmTSPAIS::preload()
{
}

	
void AlgorithmTSPAIS::endGeneration()
{
	algorithmStats(false,population,populationSize,generation,evaluations);
}

	
void AlgorithmTSPAIS::endAlgorithm()
{
	algorithmStats(true,population,populationSize,generation,evaluations);
	delete fitnessFunction;
	delete mutationOperator;
	delete selectionFunction;
	delete stopCriterion;
}

	
Individual* AlgorithmTSPAIS::createRandomIndividual(int objectives)
{
	return generateRandomIndividualInts();
}


Individual* AlgorithmTSPAIS::createEmptyIndividual(int objectives)
{
	return generateRandomIndividualInts();
}
