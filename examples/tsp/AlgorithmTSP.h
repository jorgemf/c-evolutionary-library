#ifndef ALGORITHMTSP_H_
#define ALGORITHMTSP_H_

#define MAX_NUMBER_CITIES_DISTANCE 1024

#include <string>
#include "Individual.h"

/**
 * Class with auxiliarity methods to execute the Travelling Salesman problem
 */
class AlgorithmTSP
{
		
public:

	/**
	 * Default consutrctor
	 */
	AlgorithmTSP();
	
	/**
	 * Default destructor
	 */
	virtual ~AlgorithmTSP();
	
	/**
	 * Generate a random number that is not in the array.
	 */
	static double generateRandomNumberNotInArray(double* array, int arraySize);


	/**
	 * Public enum for data type
	 */
	enum DATA_TYPE { COORDINATES_EUC_2D, COORDINATES_EUC_2D_CEIL, COORDINATES_ATT, COORDINATES_GEO_2D, MATRIX_FULL,
					 MATRIX_UPPER_ROW, MATRIX_LOWER_ROW, MATRIX_UPPER_DIAG_ROW, MATRIX_LOWER_DIAG_ROW, NONE };

	
protected:
	
	/**
	 * Returns a random individual generate from scratch. A TSPIndividual.
	 */
	Individual* generateRandomIndividualInts();
	
	/**
	 * Returns a random individual generate from scratch. A TSPIndividual2.
	 */
	Individual* generateRandomIndividualDoubles();
	
	/**
	 * Print the generation stats and saves the best individual if it is better than the last one. If the best 
	 * individual is null it will be searched in the population.
	 */
	void algorithmStats(bool last, Individual** population, int populationSize, long generation, long evaluations, Individual* best = NULL);
	
	/**
	 * Number of generations that the statics are shown
	 */
	int generationShow;
	
	/**
	 * Number of the problem cities
	 */
	int numberOfCities;
	
	/**
	 * A matrix with the cities distances
	 */
	double** citiesDistances;
	
	/**
	 * A matrix with the cities coordinates // x y z
	 */
	double** citiesCoordinates;
		 
	/**
	 * Type of the data
	 */
	DATA_TYPE dataType;
	
private:

	/**
	 * The best fitness ever found.
	 */
	double bestFitness;
	
	/**
	 * File where the best individual is saved.
	 */
	std::string saveBestFile;
	
	/**
	 * Load the cities and their distances from a file. Returns the number of cities loaded.
	 */
	int loadCities(std::string file);
	
	/**
	 * Array of int for generate random individuals.
	 */
	int* citiesNumbers;
	
	/**
	 * Calculate the euclidian distance from the coordinates of the cities. The distance is stored in the distance 
	 * matrix.
	 */
	void calculateEuclideanDistance(int cities, double** values, double** distanceMatrix);
	
	/**
	 * Calculate the euclidian distance rounded up from the coordinates of the cities . The distance is stored in the 
	 * distance matrix.
	 */
	void calculateEuclideanDistanceCeils(int cities, double** values, double** distanceMatrix);
	
	/**
	 * Calculate the geographical distance from the coordinates of the cities. The distance is stored in the distance 
	 * matrix.
	 */
	void calculateGeographicalDistance(int cities, double** values, double** distanceMatrix);
	
	/**
	 * Calculate the ATT distance from the coordinates of the cities. The distance is stored in the distance matrix.
	 */
	void calculateATTDistance(int cities, double** values, double** distanceMatrix);
	
	/**
	 * Reads a number from the input, empty lines and blanks are ignored.
	 */
	double readNextNumber(std::ifstream& input);
	
	/**
	 * Read a matrix of distances from the input. Return true if every is ok.
	 */
	bool readMatrix(std::ifstream& input, int cities, double** distanceMatrix); 
	
	/**
	 * Read a upper diagonal row matrix of distances from the input. Return true if every is ok.
	 */
	bool readUpperMatrix(std::ifstream& input, int cities, double** distanceMatrix);
	
	/**
	 * Read a lower diagonal row matrix of distances from the input. Return true if every is ok.
	 */
	bool readLowerMatrix(std::ifstream& input, int cities, double** distanceMatrix);
	
	/**
	 * Read a upper diagonal row matrix of distances from the input, with diagonal entries. Return true if every is ok.
	 */
	bool readUpperDiagMatrix(std::ifstream& input, int cities, double** distanceMatrix);
	
	/**
	 * Read a lower diagonal row matrix of distances from the input, with diagonal entries.. Return true if every is ok.
	 */
	bool readLowerDiagMatrix(std::ifstream& input, int cities, double** distanceMatrix);
	
	/**
	 * Read the coordinates from the input. Return true if every is ok.
	 */
	bool readCoordinates(std::ifstream& input, int cities, double** coordinates); 
};

#endif /*ALGORITHMTSP_H_*/
