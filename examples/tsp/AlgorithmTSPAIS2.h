#ifndef ALGORITHMTSPAIS2_H_
#define ALGORITHMTSPAIS2_H_

#include "AlgorithmTSP.h"
#include "ArtificialInmuneSystem.h"


/**
 * Concrete class for the artificial inmune algorithm in the TSP problem with double chromosome population.
 */

class AlgorithmTSPAIS2 : protected AlgorithmTSP, public ArtificialInmuneSystem
{
	
public:

	/**
	 * Default constructor.
	 */
	AlgorithmTSPAIS2();

	/**
	 * Default destructor
	 */
	virtual ~AlgorithmTSPAIS2();
	
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

#endif /*ALGORITHMTSPAIS2_H_*/
