#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include "Individual.h"

/**
 * Generic class for an algorithm.
 */
class Algorithm
{
	
protected:

	/**
	 * Defautl constructor.
	 */
	Algorithm();
	
	/**
	 * Sort the population by fitness.
	 */
	void quickSort(Individual** population, int begin, int end);
	
public:
	
	/**
	 * Default destructor.
	 */
	virtual ~Algorithm();

	/**
	 * Run the algorithm until stop criterios is reached.
	 */
	virtual void run();
	
	/**
	 * This method must initialice the operators.
	 */
	virtual void initialize();
	
	/**
	 * This method is called before the algotirhm starts, you can load a previusly population.
	 */
	virtual void preload();
	
	/**
	 * This method is called when a generation ends.
	 */
	virtual void endGeneration();
	
	/**
	 * This method is called when algorithm ends.
	 */
	virtual void endAlgorithm();
	
	/**
	 * This method must create a random invididual.
	 */
	virtual Individual* createRandomIndividual(int objectives = 0);
	
	/**
	 * This method must create a empty invididual.
	 */
	virtual Individual* createEmptyIndividual(int objectives = 0);
	
};

#endif /*ALGORITHM_H_*/
