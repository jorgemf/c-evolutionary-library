#ifndef ALGORITHMTSPAIS_H_
#define ALGORITHMTSPAIS_H_

#include "AlgorithmTSP.h"
#include "ArtificialInmuneSystem.h"


/**
 * Concrete class for the artificial inmune algorithm in the TSP problem with integer chromosome population.
 */

class AlgorithmTSPAIS : protected AlgorithmTSP, public ArtificialInmuneSystem
{
	
public:

	/**
	 * Default constructor.
	 */
	AlgorithmTSPAIS();

	/**
	 * Default destructor
	 */
	virtual ~AlgorithmTSPAIS();
	
	/**
	 * From Algorithm.initialize().
	 */
	void initialize();
	
	/**
	 * From Algorithm.preload().
	 */
	void preload();
	
	/**
	 * From Algorithm.endGeneration().
	 */
	void endGeneration();
	
	/**
	 * From Algorithm.endAlgorithm().
	 */
	void endAlgorithm();
	
	/**
	 * From Algorithm.createRandomIndividual(int).
	 */
	Individual* createRandomIndividual(int objectives);
	
	/**
	 * From Algorithm.createEmptyIndividual(int).
	 */
	Individual* createEmptyIndividual(int objectives);
};

#endif /*ALGORITHMTSPAIS_H_*/
