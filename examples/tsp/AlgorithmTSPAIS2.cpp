#include "AlgorithmTSPAIS2.h"
#include "TSPFitness.h"
#include "TSPMutation2.h"
#include "TSPSelection.h"
#include "TSPStopCriterion.h"

AlgorithmTSPAIS2::AlgorithmTSPAIS2()
{
}

AlgorithmTSPAIS2::~AlgorithmTSPAIS2()
{
}

void AlgorithmTSPAIS2::initialize()
{
	fitnessFunction = new TSPFitness(this->citiesDistances, this->citiesCoordinates, this->dataType);
	mutationOperator = new TSPMutation2();
	selectionFunction = new TSPSelection();
	stopCriterion = new TSPStopCriterion();
}

	
void AlgorithmTSPAIS2::preload()
{
}

	
void AlgorithmTSPAIS2::endGeneration()
{
	algorithmStats(false,population,populationSize,generation,evaluations);
}

	
void AlgorithmTSPAIS2::endAlgorithm()
{
	algorithmStats(true,population,populationSize,generation,evaluations);
	delete fitnessFunction;
	delete mutationOperator;
	delete selectionFunction;
	delete stopCriterion;
}

	
Individual* AlgorithmTSPAIS2::createRandomIndividual(int objectives)
{
	return generateRandomIndividualDoubles();
}


Individual* AlgorithmTSPAIS2::createEmptyIndividual(int objectives)
{
	return generateRandomIndividualDoubles();
}
