#ifndef MULTIOBJECTIVEEVALUATION_H_
#define MULTIOBJECTIVEEVALUATION_H_

#include "Individual.h"

/**
 * A class which evaluate multiobjective individuals.
 */
class MultiObjectiveEvaluation
{
	
protected:

	/**
	 * Default constructor.
	 */
	MultiObjectiveEvaluation();
	
public:

	/**
	 * Default destructor.
	 */
	virtual ~MultiObjectiveEvaluation();
	
	/**
	 * A method to assing the objectives values to an individual.
	 */
	virtual void evaluate(Individual* individual);
};

#endif /*MULTIOBJECTIVEEVALUATION_H_*/
