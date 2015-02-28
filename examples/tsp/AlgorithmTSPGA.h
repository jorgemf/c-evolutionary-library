#ifndef ALGORITHMTSPGA_H_
#define ALGORITHMTSPGA_H_

#include "AlgorithmTSP.h"
#include "GeneticAlgorithm.h"


/**
 * Concrete class for the genetic algorithm in the TSP problem with integer chromosome population.
 */
class AlgorithmTSPGA : protected AlgorithmTSP, public GeneticAlgorithm
{
	
public:

	/**
	 * Default constructor.
	 */
	AlgorithmTSPGA();
	
	/**
	 * Default destructor.
	 */
	virtual ~AlgorithmTSPGA();
	
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

#endif /*ALGORITHMTSPGA_H_*/
