#ifndef ARTIFICIALNMUNESYSTEM_H_
#define ARTIFICIALNMUNESYSTEM_H_

#include "Algorithm.h"
#include "FitnessFunction.h"
#include "Individual.h"
#include "Mutation.h"
#include "Selection.h"
#include "StopCriterion.h"

/**
 * Default template for Artificial Inmune System Algorithms
 */
class ArtificialInmuneSystem : public Algorithm
{
	
protected:

	/**
	 * Default constructor, this class can not be directly instanciated.
	 */
	ArtificialInmuneSystem();
	
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
	 * This method initialice the artifial inmune system algorithm params.
	 */
	void initializeParams();
	
	
public:

	/**
	 * Default destructor
	 */
	virtual ~ArtificialInmuneSystem();

	/**
	 * Run the algorithm until stop criterios is reached.
	 */
	void run();
		
};

#endif /*ARTIFICIALNMUNESYSTEM_H_*/
