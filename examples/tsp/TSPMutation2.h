#ifndef TSPMUTATION2_H_
#define TSPMUTATION2_H_

#include "TSPMutation.h"
#include "TSPIndividual2.h"


/**
 * Class which mutates a TSPIndividual2.
 */
class TSPMutation2 : public TSPMutation
{
	
private:
	
	/**
	 * Exchange two regions. The region size is choosen as regionSize method explain.
	 */
	void exchangeRegions(TSPIndividual2* individual);
	
	/**
	 * Reverse a region. The region size is choosen between 0 and number of cities.
	 */
	void reverseRegion(TSPIndividual2* individual);
	
	/**
	 * Change a number in the array of doubles. The new number is not in the array.
	 */
	void mutateNumber(TSPIndividual2* individual);
	
public:

	/**
	 * Default constructor.
	 */
	TSPMutation2();
	
	/**
	 * Default destructor.
	 */
	virtual ~TSPMutation2();
	
	/**
	 * From Mutation.mutate(Individual*)
	 */
	void mutate(Individual* individual);
};

#endif /*TSPMUTATION2_H_*/
