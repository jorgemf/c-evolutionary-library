#ifndef GENETICALGORITHM_H_
#define GENETICALGORITHM_H_

#include "Algorithm.h"
#include "Crossover.h"
#include "FitnessFunction.h"
#include "Individual.h"
#include "Mutation.h"
#include "Selection.h"
#include "StopCriterion.h"

/**
 * Default template for Genetic Algorithms
 */
class GeneticAlgorithm : public Algorithm
{
	
protected:

	/**
	 * Default constructor, this class can not be directly instanciated.
	 */
	GeneticAlgorithm();
	
	/**
	 * Crossover operator.
	 */
	Crossover* crossoverOperator;
	
	/**
	 * Fitness function.
	 */
	FitnessFunction* fitnessFunction;
	
	/**
	 * Mutation operator.
	 */
	Mutation* mutationOperator;
	
	/**
	 * Selection function.
	 */
	Selection* selectionFunction;
	
	/**
	 * Stop criterion 
	 */
	StopCriterion* stopCriterion;
	
	/**
	 * Individuals population 
	 */
	Individual** population;
	
	/**
	 * Individuals intermediate population 
	 */
	Individual** intermediatePopulation;
	
	/**
	 * Population size.
	 */
	int populationSize;	
	
	/**
	 * Number of the generation.
	 */
	int generation;
	
	/**
	 * Number of individuals that have been evaluated.
	 */
	long evaluations;
	
	
private:
	
	/**
	 * Number of elitism individuals in each new population 
	 */
	int elitismRate;
	
	/**
	 * Number of crossover individuals in each new population 
	 */
	int crossoverRate;
	
	/**
	 * Number of mutation individuals in each new population 
	 */
	int mutationRate;
	
	/**
	 * This method initialice the genetic algorithm params.
	 */
	void initializeParams();
	
public:

	/**
	 * Default destructor
	 */
	virtual ~GeneticAlgorithm();

	/**
	 * Run the algorithm until stop criterios is reached.
	 */
	void run();
	
};

#endif /*GENETICALGORITHM_H_*/
