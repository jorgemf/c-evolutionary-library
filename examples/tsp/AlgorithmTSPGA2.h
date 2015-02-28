#ifndef ALGORITHMTSPGA2_H_
#define ALGORITHMTSPGA2_H_

#include "AlgorithmTSP.h"
#include "GeneticAlgorithm.h"


/**
 * Concrete class for the genetic algorithm in the TSP problem with double chromosome population.
 */
class AlgorithmTSPGA2 : protected AlgorithmTSP, public GeneticAlgorithm
{
	
public:

	/**
	 * Default constructor.
	 */
	AlgorithmTSPGA2();
	
	/**
	 * Default destructor.
	 */
	virtual ~AlgorithmTSPGA2();
	
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

#endif /*ALGORITHMTSPGA2_H_*/
