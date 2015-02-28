#ifndef TSPMUTATION_H_
#define TSPMUTATION_H_

#include "Mutation.h"
#include "TSPIndividual.h"


/**
 * Class which mutates a TSPIndividual.
 */
class TSPMutation : public Mutation
{
	
private:

	/**
	 * The number of cities.
	 */
	int numberOfCities;
	
	/**
	 * Array of probabilities for faster operations in regionSize method.
	 */
	double* probabilities;
	
	/**
	 * Minimal number of mutations.
	 */
	int minMutations;
	
	/**
	 * Maximal number of mutations.
	 */
	int maxMutations;
	
	/**
	 * Exchange two regions. The region size is choosen as regionSize method explain.
	 */
	void exchangeRegions(TSPIndividual* individual);
	
	/**
	 * Reverse a region. The region size is choosen between 0 and number of cities.
	 */
	void reverseRegion(TSPIndividual* individual);

	
protected:

	/**
	 * Returns a region size. The size is alwais lower than one third of the number of cities of the individual. The 
	 * probability to choose a region size of size X is (where N is 5/12 of number of cities):
	 * P(X) = ( 1 / X ) / sum{from i=1 to N}{ 1/i }
	 */
	int regionSize(TSPIndividual* individual);


public:

	/**
	 * Default constructor.
	 */
	TSPMutation();
	
	/**
	 * Default destructor.
	 */
	virtual ~TSPMutation();
	
	/**
	 * From Mutation.mutate(Individual*)
	 */
	void mutate(Individual* individual);
	
	/**
	 * From Mutation.mutateRange(Individual*, double)
	 */
	void mutateRange(Individual* individual, double mutation);
};

#endif /*TSPMUTATION_H_*/
