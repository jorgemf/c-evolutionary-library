#ifndef TSPINDIVIDUAL2_H_
#define TSPINDIVIDUAL2_H_


#include <string>
#include "TSPIndividual.h"

/**
 * Class which represents a route of cities. The routes is a random keys vector.
 */
class TSPIndividual2 : public TSPIndividual
{
	
public:

	/**
	 * Default constructor.
	 */
	TSPIndividual2(int numberOfCities);
	
	/**
	 * Default destructor.
	 */
	virtual ~TSPIndividual2();
	
	/**
	 * Return an array with the route of the cities.
	 */
	int* getCitiesRoute();
	
	/**
	 * From Individual.copy(Individual*)
	 */
	void copy(Individual* individual);
	
	/**
	 * Returns the cities array in double format.
	 */
	inline double* getDoubleRoute(){
		return this->citiesDouble;
	}
	
	
protected:
	
	/**
	 * The cities route.
	 */
	double* citiesDouble;
	
	/**
	 * Auxiliar array to sort the cities double.
	 */
	double* auxiliar;
	
	/**
	 * Sorts an array of doubles, it is supposed that all numbers are differents.
	 */
	void quickSort(double* array, int begin, int end);
	
	/**
	 * Performs a binary search in an array of doubles. If the value is not found it returns -1.
	 */
	int binarySearch(double* array, double searchValue, int begin, int end);
	
};


#endif /*TSPINDIVIDUAL2_H_*/
