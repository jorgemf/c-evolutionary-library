#ifndef MUTATION_H_
#define MUTATION_H_

#include "Individual.h"


/**
 * Class which mutates individuals.
 */
class Mutation
{
	
protected:

	/**
	 * Default constructor.
	 */
	Mutation();
	
	
public:
	
	/**
	 * Default destructor.
	 */
	virtual ~Mutation();
	
	/**
	 * Function that mutates an individual.
	 */
	virtual void mutate(Individual* individual);
	
	/**
	 * Function that mutates an individual, but mutation degree is given by mutation: 1 means maximal mutation
	 * and 0 minimal. 
	 */
	virtual void mutateRange(Individual* individual, double mutation);
	
};

#endif /*MUTATION_H_*/
