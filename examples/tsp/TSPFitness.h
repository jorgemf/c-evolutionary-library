#ifndef TSPFITNESS_H_
#define TSPFITNESS_H_

#include "FitnessFunction.h"
#include "AlgorithmTSP.h"


/**
 * Class which evaluates a TSPIndividual. 
 */
class TSPFitness : public FitnessFunction
{

private:

	/**
	 * The matrix with the distances.
	 */
	double** distanceMatrix;
	
	/**
	 * A matrix with the coordinates. x y z
	 */
	double** coordinatesMatrix;
	
	/**
	 * Data type, for fitness function.
	 */
	AlgorithmTSP::DATA_TYPE dataType;
	
	/**
	 * Calculate the euclidian distance from the coordinates of the cities.
	 */
	double evaluateEuclideanDistance(int* cities, int numberOfCities);
	
	/**
	 * Calculate the euclidian distance rounded up from the coordinates of the cities .
	 */
	double evaluateEuclideanDistanceCeils(int* cities, int numberOfCities);
	
	/**
	 * Calculate the geographical distance from the coordinates of the cities. 
	 */
	double evaluateGeographicalDistance(int* cities, int numberOfCities);
	
	/**
	 * Calculate the ATT distance from the coordinates of the cities.
	 */
	double evaluateATTDistance(int* cities, int numberOfCities);
	
public:

	/**
	 * Default constructor. The matrix with the distances must be numberOfCities of height and width.
	 */
	TSPFitness(double** distanceMatrix, double** coordinatesMatrix, AlgorithmTSP::DATA_TYPE dataType);

	/**
	 * Default destructor.
	 */
	virtual ~TSPFitness();
	
	/**
	 * From FitnessFunction.evaluate(Individual*)
	 */
	double evaluate(Individual* individual);
};

#endif /*TSPFITNESS_H_*/
