#ifndef TSPCROSSOVER_H_
#define TSPCROSSOVER_H_

#include "Crossover.h"

/**
 * Class which crosses two TSPIndividual into a third one.
 */
class TSPCrossover : public Crossover
{
	
public:

	/**
	 * Default constructor.
	 */
	TSPCrossover();
	
	/**
	 * Default destructor.
	 */
	virtual ~TSPCrossover();
	
	/**
	 * From Crossover.cross(Individual*,Individual*,Individual*)
	 */
	void cross(Individual* c1, Individual* c2, Individual* offspring);
};

#endif /*TSPCROSSOVER_H_*/
