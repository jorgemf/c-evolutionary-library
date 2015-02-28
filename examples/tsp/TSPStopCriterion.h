#ifndef TSPSTOPCRITERION_H_
#define TSPSTOPCRITERION_H_

#include "StopCriterion.h"

/**
 * A class which represents a stop criterior to finish an algorithm.
 */
class TSPStopCriterion : public StopCriterion
{
	
public:

	/**
	 * Default constructor.
	 */
	TSPStopCriterion();

	/**
	 * Default destructor.
	 */
	virtual ~TSPStopCriterion();
	
	/**
	 * From StopCriterion.stop(int,Individual**,int,long);
	 */
	bool stop(int generation, Individual **population, int populationSize, long evaluations);
	
	
private:	
	
	/**
	 * Maximal number of evaluations
	 */
	long maxEvaluaions;
};

#endif /*TSPSTOPCRITERION_H_*/
