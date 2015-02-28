#ifndef TSPSELECTION_H_
#define TSPSELECTION_H_

#include "Selection.h"

/**
 * Class which represents a selection algorithm to select an individual from a population.
 */
class TSPSelection : public Selection
{
	
public:

	/**
	 * Default constructor.
	 */
	TSPSelection();

	/**
	 * Default destructor.
	 */
	virtual ~TSPSelection();
	
	/**
	 * From Selection.selectIndividual(Individual**,int)
	 */
	virtual Individual* selectIndividual(Individual** population, int populationSize);
	
	/**
	 * From Selection.selectIndividualToDie(Individual**,int)
	 */
	 virtual Individual* selectIndividualToDie(Individual** population, int populationSize);
	 
	 
private:

	/**
	 * Size of the tournament for selecting an individual.
	 */
	int tournamentSize;

	/**
	 * Size of the tournament for selecting an individual to die.
	 */
	int tournamentSizeToDie;
	
	/**
	 * Selects a random individual from a population.
	 */
	Individual* selectRandomIndividual(Individual** population, int populationSize); 
	 
};

#endif /*TSPSELECTION_H_*/
