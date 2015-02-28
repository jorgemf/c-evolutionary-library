#ifndef CROSSOVER_H_
#define CROSSOVER_H_

#include "Individual.h"

/**
 * Class which crosses two individuals into a third one.
 */
class Crossover
{
	
protected:

	/**
	 * Default constructor.
	 */
	Crossover();
	
	
public:
	
	/**
	 * Default destructor.
	 */
	virtual ~Crossover();
	
	/**
	 * Function that crosses individuals c1 and c2 and stores the results one in the offspring variable.
	 */
	virtual void cross(Individual* c1, Individual* c2, Individual* offspring);
};

#endif /*CROSSOVER_H_*/
