#ifndef TSPINDIVIDUAL_H_
#define TSPINDIVIDUAL_H_

#include <string>
#include "Individual.h"

/**
 * Class which represents a route of cities.
 */
class TSPIndividual : public Individual
{
	
public:

	/**
	 * Default constructor.
	 */
	TSPIndividual(int numberOfCities);
	
	/**
	 * Default destructor.
	 */
	virtual ~TSPIndividual();
	
	/**
	 * Saves the route into a file.
	 */
	bool saveRouteFile(std::string file, long evaluations);
	
	/**
	 * From Individual.copy(Individual*)
	 */
	void copy(Individual* individual);
	
	/**
	 * Returns the number of cities.
	 */
	inline int getNumberCities()
	{
		return this->numberOfCities;	
	}
	
	/**
	 * Return an array with the route of the cities.
	 */
	virtual int* getCitiesRoute();
	
protected:

	/**
	 * Number of cities.
	 */
	int numberOfCities;
	
	/**
	 * The cities route.
	 */
	int* cities;
	
};

#endif /*TSPINDIVIDUAL_H_*/
