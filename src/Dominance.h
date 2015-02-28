#ifndef DOMINANCE_H_
#define DOMINANCE_H_

#include "Individual.h"


/**
 * Class which represents a dominance algorithm to set the dominances in a population.
 */
class Dominance
{
	
protected:

	/**
	 * Default constructor.
	 */
	Dominance();
	
public:

	/**
	 * Default destructor.
	 */
	virtual ~Dominance();
	
	/**
	 * Method to assing dominances. Dominance must be initialized at 0 and the top value must be less than population 
	 * size.
	 */
	virtual void assignDominance(Individual** population, int populationSize);
};

#endif /*DOMINANCE_H_*/
