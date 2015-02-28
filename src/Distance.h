#ifndef DISTANCE_H_
#define DISTANCE_H_

#include "Individual.h"


/**
 * Class which represents a distancie function to assign distances among population individuals.
 */
class Distance
{
	
protected:

	/**
	 * Default constructor.
	 */
	Distance();
	
public:

	/**
	 * Default destructor.
	 */
	virtual ~Distance();
	
	/**
	 * Method to assign distances among individuals. The distance must be normalized between 0 and 1.
	 */
	virtual void assignDistances(Individual** population, int populationSize);
	
};

#endif /*DISTANCE_H_*/
