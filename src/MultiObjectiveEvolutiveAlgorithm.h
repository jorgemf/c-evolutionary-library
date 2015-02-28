#ifndef MULTIOBJECTIVEEVOLUTIVEALGORITHM_H_
#define MULTIOBJECTIVEEVOLUTIVEALGORITHM_H_

#include "Algorithm.h"
#include "Individual.h"
#include "Crossover.h"
#include "Mutation.h"
#include "Selection.h"
#include "StopCriterion.h"
#include "Dominance.h"
#include "Distance.h"
#include "MultiObjectiveEvaluation.h"

/**
 * Default template for Multi-Objective Evolutive Algorithms
 */
class MultiObjectiveEvolutiveAlgorithm : public Algorithm
{
	
protected:

	/**
	 * Default constructor.
	 */
	MultiObjectiveEvolutiveAlgorithm();
	
	/**
	 * Multi-Objective evaluation function.
	 */
	MultiObjectiveEvaluation* multiObjectiveEvaluation;
	
	/**
	 * Distance function.
	 */
	Distance* distanceFunction;
	
	/**
	 * Dominance operator.
	 */
	Dominance* dominanceOperator;
	
	/**
	 * Crossover operator.
	 */
	Crossover* crossoverOperator;
	
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
	 * Individuals intermediate population 
	 */
	Individual** intermediatePopulation;
	
	/**
	 * Individuals population 
	 */
	Individual** population;
	
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
	 * This method initialice the multi-objective evolutive algorithm params.
	 */
	void initializeParams();
	
	
public:

	/**
	 * Default destructor.
	 */
	virtual ~MultiObjectiveEvolutiveAlgorithm();

	/**
	 * Run the algorithm until stop criterios is reached.
	 */
	void run();
};

#endif /*MULTIOBJECTIVEEVOLUTIVEALGORITHM_H_*/
