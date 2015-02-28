#ifndef FITNESSFUNCTION_H_
#define FITNESSFUNCTION_H_

#include "Individual.h"

/**
 * Class which evaluates an individual. 
 */
class FitnessFunction
{
	
protected:

	/**
	 * Default constructor.
	 */
	FitnessFunction();
	
	
public:

	/**
	 * Default destructor.
	 */
	virtual ~FitnessFunction();
	
	/**
	 * Main function, it evaluates an individual and returns its fitness.
	 */
	virtual double evaluate(Individual* individual);
};

#endif /*FITNESSFUNCTION_H_*/
