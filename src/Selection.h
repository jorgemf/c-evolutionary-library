#ifndef SELECTION_H_
#define SELECTION_H_

#include "Individual.h"

/**
 * Class which represents a selection algorithm to select an individual from a population.
 */
class Selection
{
	
protected:

	/**
	 * Default constructor.
	 */
	Selection();
	
	
public:

	/**
	 * Default destructor.
	 */
	virtual ~Selection();
	
	/**
	 * Main function, returns a pointer to the selected individual.
	 */
	virtual Individual* selectIndividual(Individual** population, int populationSize);
	
	/**
	 * Function that selects the next individual to die.
	 */
	virtual Individual* selectIndividualToDie(Individual** population, int populationSize);
};

#endif /*SELECTION_H_*/
