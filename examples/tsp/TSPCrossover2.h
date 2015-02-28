#ifndef TSPCROSSOVER2_H_
#define TSPCROSSOVER2_H_

#include "Crossover.h"

/**
 * Class which crosses two TSPIndividual into a third one.
 */
class TSPCrossover2 : public Crossover
{
	
public:

	/**
	 * Default constructor.
	 */
	TSPCrossover2();
	
	/**
	 * Default destructor.
	 */
	virtual ~TSPCrossover2();
	
	/**
	 * From Crossover.cross(Individual*,Individual*,Individual*)
	 */
	void cross(Individual* c1, Individual* c2, Individual* offspring);
};

#endif /*TSPCROSSOVER2_H_*/
