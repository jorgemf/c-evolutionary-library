#ifndef STOPCRITERION_H_
#define STOPCRITERION_H_

#include "Individual.h"

/**
 * A class which represents a stop criterior to finish an algorithm.
 */
class StopCriterion
{
	
protected:

	/**
	 * Default constructor.
	 */
	StopCriterion();
	
	
public:

	/**
	 * Default destructor.
	 */
	virtual ~StopCriterion();
	
	/**
	 * Main function that returns false when the algorithm must stop.
	 */
	virtual bool stop(int generation, Individual **population, int populationSize, long evaluations);
};

#endif /*STOPCRITERION_H_*/
