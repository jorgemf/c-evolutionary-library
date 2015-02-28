#include "AlgorithmTSP.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <limits>


#include "TSPIndividual.h"
#include "TSPIndividual2.h"
#include "PropertiesMap.h"
#include "mtrand.h"

AlgorithmTSP::AlgorithmTSP()
{
	string aux;
	PropertiesMap* properties;
	properties = PropertiesMap::instance();

	// maximal fitness posible
//	bestFitness = std::numeric_limits<double>::max();
	bestFitness = 1.7E308;

	// file where the best is saved
	saveBestFile = properties->get("saveBest");

	aux = properties->get("generationShow");
	if(aux.length() > 0){
		char *finalPtr;
		finalPtr = 0;
		generationShow = strtol(aux.c_str(),&finalPtr,10);
	}

	string citiesFile = properties->get("citiesFile");

	this->numberOfCities = 0;
	this->numberOfCities = loadCities(citiesFile);
	if(this->numberOfCities <= 0){
		std::cerr << "No cities loaded" << std::endl;
	}else{
		this->citiesNumbers = new int[this->numberOfCities];
	}
}

AlgorithmTSP::~AlgorithmTSP()
{
	if(this->numberOfCities > 0){
		for(int i = 0; i < this->numberOfCities; i++){
			delete this->citiesDistances[i];
			delete this->citiesCoordinates[i];
		}
		delete this->citiesDistances;
		delete this->citiesNumbers;
		delete this->citiesCoordinates;
	}
}


Individual* AlgorithmTSP::generateRandomIndividualInts()
{
	MTRand_int32 irand;

	int numberOfCities = this->numberOfCities;
	for(int i = 0; i < numberOfCities; i++){
		this->citiesNumbers[i] = i+1;
	}

	TSPIndividual* individual = new TSPIndividual(numberOfCities);
	int* cities = individual->getCitiesRoute();

	for(int i = 0; i < this->numberOfCities; i++){
		int random = irand() % numberOfCities;
		cities[i] = this->citiesNumbers[random];
		numberOfCities--;
		this->citiesNumbers[random] = this->citiesNumbers[numberOfCities];
	}

	return individual;
}


Individual* AlgorithmTSP::generateRandomIndividualDoubles()
{
	TSPIndividual2* individual = new TSPIndividual2(numberOfCities);
	double* array = individual->getDoubleRoute();
	for(int i = 0; i < numberOfCities; i++){
		array[i] = generateRandomNumberNotInArray(array,i);
	}
	return individual;
}


void AlgorithmTSP::algorithmStats(bool last, Individual** population, int populationSize, long generation, long evaluations, Individual* best)
{
	if(best == NULL && populationSize > 0){
		best = population[0];
		for(int i = 1; i < populationSize; i++){
			if(population[i]->getFitness() < best->getFitness()){
				best = population[i];
			}
		}
	}

	if( (this->bestFitness > best->getFitness()) ){
		this->bestFitness = best->getFitness();
		if(!((TSPIndividual*)best)->saveRouteFile(saveBestFile,evaluations)){
			std::cerr << "Can not save the best individual in the file: " << saveBestFile << std::endl;
		}
	}
	if(generation % generationShow == 0 || last){

		std::cout << "# Generation: " << generation
			<< "\tEvaluations: " << evaluations
			<< "\tBest Fitness Ever: " << bestFitness;

		double generationAverageFitness = 0;
		double generationMaxFitness = 0;
		double generationMinFitness = 0;

		if(populationSize > 0){

			generationAverageFitness = population[0]->getFitness();
			generationMaxFitness = population[0]->getFitness();
			generationMinFitness = population[0]->getFitness();

			for(int i = 1; i < populationSize; i++){
			generationAverageFitness += population[i]->getFitness();
				if(generationMaxFitness < population[i]->getFitness()){
					generationMaxFitness = population[i]->getFitness();
				}else if (generationMinFitness > population[i]->getFitness()){
					generationMinFitness = population[i]->getFitness();
				}
			}

			generationAverageFitness = generationAverageFitness / populationSize;

			std::cout << "\tFitness \tBest: " << generationMinFitness
				<< "\tWorst: " << generationMaxFitness
				<< "\tAverage: " << generationAverageFitness;
		}
		std::cout << std::endl;
		std::cout << generation << " " << evaluations << " " << bestFitness ;
		if(populationSize > 0){
			std::cout << " " << generationMinFitness << " " << generationMaxFitness << " " << generationAverageFitness;
		}
		std::cout << std::endl;
	}
}


/**
 * Load the cities and their distances from a file.
 */
int AlgorithmTSP::loadCities(std::string file)
{
	this->dataType = NONE;

	std::string line;
	if(this->numberOfCities > 0){
		for(int i = 0; i < this->numberOfCities; i++){
			delete this->citiesDistances[i];
			delete this->citiesCoordinates[i];
		}
		delete this->citiesDistances;
		delete this->citiesCoordinates;
		this->numberOfCities = -1;
	}



	bool ok = false;
	std::ifstream input;
	input.open(file.c_str());
	ok = input.good();
	if(ok){
		bool cont = true;
		while(!input.eof() && ok && cont){
			getline(input,line);
			std::cout << "# " << line << std::endl;
			// number of cities
			if(line.find("DIMENSION") != string::npos){
			   	int pos = line.find(":");
			   	std::string number = line.substr(pos+1,line.length()-(pos+1));
				char* finalPtr = 0;
			   	this->numberOfCities = strtol(number.c_str(),&finalPtr, 10);
					std::cout << "number of cities: " << this->numberOfCities << std::endl;
			}
			// data section type
			if(line.find("EDGE_WEIGHT_TYPE") != string::npos){
				if(line.rfind("EUC_2D") != string::npos){
					this->dataType = COORDINATES_EUC_2D;
					std::cout << "data type: EUC_2D" << std::endl;
				}else if(line.rfind("CEIL_2D") != string::npos){
					this->dataType = COORDINATES_EUC_2D_CEIL;
					std::cout << "data type: CEIL_2D" << std::endl;
				}else if(line.rfind("GEO") != string::npos){
					this->dataType = COORDINATES_GEO_2D;
					std::cout << "data type: GEO" << std::endl;
				}else if(line.rfind("ATT") != string::npos){
					this->dataType = COORDINATES_ATT;
					std::cout << "data type: ATT" << std::endl;
				}else if(line.rfind("EXPLICIT") == string::npos){
					std::cerr << "Error: unkown data type" << std::endl;
				}
			}else if(line.find("EDGE_WEIGHT_FORMAT") != string::npos){
				if(line.rfind("FULL_MATRIX") != string::npos){
					this->dataType = MATRIX_FULL;
					std::cout << "matrix: FULL" << std::endl;
				}else if(line.rfind("UPPER_ROW") != string::npos){
					this->dataType = MATRIX_UPPER_ROW;
					std::cout << "matrix: UPPER_ROW" << std::endl;
				}else if(line.rfind("LOWER_ROW") != string::npos){
					this->dataType = MATRIX_LOWER_ROW;
					std::cout << "matrix: LOWER_ROW" << std::endl;
				}else if(line.rfind("UPPER_DIAG_ROW") != string::npos){
					this->dataType = MATRIX_UPPER_DIAG_ROW;
					std::cout << "matrix: UPPER_DIAG_ROW" << std::endl;
				}else if(line.rfind("LOWER_DIAG_ROW") != string::npos){
					this->dataType = MATRIX_LOWER_DIAG_ROW;
					std::cout << "matrix: LOWER_DIAG_ROW" << std::endl;
				}else{
					std::cerr << "Error: unkown data type" << std::endl;
				}
			}
			// data section
			if(line.find("NODE_COORD_SECTION") != string::npos ||
			   line.find("DISPLAY_DATA_SECTION") != string::npos ||
			   line.find("EDGE_DATA_SECTION") != string::npos ||
			   line.find("EDGE_WEIGHT_SECTION") != string::npos ){
				cont = false;
			}

		}
		if(this->numberOfCities > 0){
			this->citiesDistances = new double*[this->numberOfCities];
			for(int i = 0; i < this->numberOfCities; i++){
				this->citiesDistances[i] = new double[this->numberOfCities];
			}
			this->citiesCoordinates = new double*[this->numberOfCities];
			for(int i = 0; i < this->numberOfCities; i++){
				this->citiesCoordinates[i] = new double[3];
			}
			switch(this->dataType){
				case COORDINATES_EUC_2D:
					readCoordinates(input, this->numberOfCities, this->citiesCoordinates);
					if(this->numberOfCities <= MAX_NUMBER_CITIES_DISTANCE){
						calculateEuclideanDistance(this->numberOfCities, this->citiesCoordinates, this->citiesDistances);
					}
					break;
				case COORDINATES_GEO_2D:
					readCoordinates(input, this->numberOfCities, this->citiesCoordinates);
					calculateGeographicalDistance(this->numberOfCities, this->citiesCoordinates, this->citiesDistances);
					break;
				case COORDINATES_EUC_2D_CEIL:
					readCoordinates( input, this->numberOfCities, this->citiesCoordinates);
					calculateEuclideanDistanceCeils(this->numberOfCities, this->citiesCoordinates, this->citiesDistances);
					break;
				case COORDINATES_ATT:
					readCoordinates(input, this->numberOfCities, this->citiesCoordinates);
					calculateATTDistance(this->numberOfCities, this->citiesCoordinates, this->citiesDistances);
					break;
				case MATRIX_FULL:
					readMatrix(input, this->numberOfCities, this->citiesDistances);
					break;
				case MATRIX_UPPER_ROW:
					readUpperMatrix(input, this->numberOfCities, this->citiesDistances);
					break;
				case MATRIX_LOWER_ROW:
					readLowerMatrix(input, this->numberOfCities, this->citiesDistances);
					break;
				case MATRIX_UPPER_DIAG_ROW:
					readUpperDiagMatrix(input, this->numberOfCities, this->citiesDistances);
					break;
				case MATRIX_LOWER_DIAG_ROW:
					readLowerDiagMatrix(input, this->numberOfCities, this->citiesDistances);
					break;
				case NONE:
					std::cerr << "Error: no data loaded" << std::endl;
					break;
			}
		}
	}
	input.close();
	return this->numberOfCities;
}


double AlgorithmTSP::generateRandomNumberNotInArray(double* array, int arraySize)
{
	MTRand rand;
	double value;
	bool inArray;
	do{
		inArray = false;
		value = rand();
		int i = 0;
		while(i < arraySize && !inArray){
			inArray = array[i] == value;
			i++;
		}
	}while(inArray);
	return value;
}


void AlgorithmTSP::calculateEuclideanDistance(int cities, double** values, double** distanceMatrix)
{
	double xd;
	double yd;
	for(int i = 0; i < cities; i++){
		distanceMatrix[i][i] = 0;
		for(int j = i+1; j < cities; j++){
			xd = values[i][0] - values[j][0];
			yd = values[i][1] - values[j][1];
			distanceMatrix[i][j] = (int)( sqrt( xd*xd + yd*yd ) );
			distanceMatrix[j][i] = distanceMatrix[i][j];
		}
	}
}


void AlgorithmTSP::calculateEuclideanDistanceCeils(int cities, double** values, double** distanceMatrix)
{
	for(int i = 0; i < cities; i++){
		distanceMatrix[i][i] = 0;
		for(int j = i+1; j < cities; j++){
			double xd = values[i][0] - values[j][0];
			double yd = values[i][1] - values[j][1];
			distanceMatrix[i][j] = ceil( sqrt( xd*xd + yd*yd ) );
			distanceMatrix[j][i] = distanceMatrix[i][j];
		}
	}
}


void AlgorithmTSP::calculateGeographicalDistance(int cities, double** values, double** distanceMatrix)
{
	double PI = 3.141592;
	double RRR = 6378.388;
	// change the values of coordinates for latitude and longitude
	for(int i = 0; i < cities; i++){
		int deg = (int)values[i][0];
		double min = values[i][0]-deg;
		values[i][0] = PI * (deg + 5.0 * min / 3.0) / 180.0; // latitude
		deg = (int)values[i][1];
		min = values[i][1]-deg;
		values[i][1] = PI * (deg + 5.0 * min / 3.0) / 180.0; // longitude
	}
	for(int i = 0; i < cities; i++){
		for(int j = 0; j < cities; j++){
			double q1 = cos( values[i][1] - values[j][1]);
			double q2 = cos( values[i][0] - values[j][0]);
			double q3 = cos( values[i][0] + values[j][0]);
			distanceMatrix[i][j] = (int)( RRR * acos(0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0);
		}
	}
}


void AlgorithmTSP::calculateATTDistance(int cities, double** values, double** distanceMatrix)
{
	for(int i = 0; i < cities; i++){
		distanceMatrix[i][i] = 0;
		for(int j = i+1; j < cities; j++){
			double xd = values[i][0] - values[j][0];
			double yd = values[i][1] - values[j][1];
			double r =  sqrt( (xd*xd + yd*yd) / 10.0 );
			double t = (int)r;
			if(t<r){
				distanceMatrix[i][j] = t+1;
			}else{
				distanceMatrix[i][j] = t;
			}
			distanceMatrix[j][i] = distanceMatrix[i][j];
		}
	}
}


double AlgorithmTSP::readNextNumber(std::ifstream& input)
{
	double num = 0;
	char c;
	string str = "";

	if(input.good()){
		c = input.get();
		// skip blanks and end of lines
		while(input.good() && (c == ' ' || c == '\n' || c == '\r')){
			c = input.get();
		}
		while(input.good() && c != ' ' && c != '\n' && c != '\r'){
			str += c;
			c = input.get();
		}
		num = atof(str.c_str());
	}
	return num;
}


bool AlgorithmTSP::readMatrix(std::ifstream& input, int cities, double** distanceMatrix)
{
	bool ok = true;
	for(int i = 0; i < cities; i++){
		for(int j = 0; j < cities; j++){
			double nextNumber = 0;
			if(input.eof()){
				ok = false;
			}else{
				nextNumber = this->readNextNumber(input);
			}
			distanceMatrix[i][j] = nextNumber;
		}
	}
	return ok;
}


bool AlgorithmTSP::readUpperDiagMatrix(std::ifstream& input, int cities, double** distanceMatrix)
{
	bool ok = true;
	for(int i = 0; i < cities; i++){
		for(int j = i; j < cities; j++){
			double nextNumber = 0;
			if(input.eof()){
				ok = false;
			}else{
				nextNumber = this->readNextNumber(input);
			}
			distanceMatrix[i][j] = nextNumber;
			distanceMatrix[j][i] = nextNumber;
		}
	}
	return ok;
}


bool AlgorithmTSP::readLowerDiagMatrix(std::ifstream& input, int cities, double** distanceMatrix)
{
	bool ok = true;
	for(int i = 0; i < cities; i++){
		for(int j = 0; j <= i; j++){
			double nextNumber = 0;
			if(input.eof()){
				ok = false;
			}else{
				nextNumber = this->readNextNumber(input);
			}
			distanceMatrix[i][j] = nextNumber;
			distanceMatrix[j][i] = nextNumber;
		}
	}
	return ok;
}


bool AlgorithmTSP::readUpperMatrix(std::ifstream& input, int cities, double** distanceMatrix)
{
	bool ok = true;
	for(int i = 0; i < cities; i++){
		for(int j = i+1; j < cities; j++){
			double nextNumber = 0;
			if(input.eof()){
				ok = false;
			}else{
				nextNumber = this->readNextNumber(input);
			}
			distanceMatrix[i][j] = nextNumber;
			distanceMatrix[j][i] = nextNumber;
		}
	}
	return ok;
}


bool AlgorithmTSP::readLowerMatrix(std::ifstream& input, int cities, double** distanceMatrix)
{
	bool ok = true;
	for(int i = 0; i < cities; i++){
		for(int j = 0; j < i; j++){
			double nextNumber = 0;
			if(input.eof()){
				ok = false;
			}else{
				nextNumber = this->readNextNumber(input);
			}
			distanceMatrix[i][j] = nextNumber;
			distanceMatrix[j][i] = nextNumber;
		}
	}
	return ok;
}


bool AlgorithmTSP::readCoordinates(std::ifstream& input, int cities, double** coordinates)
{
	bool ok = true;
	bool* citiesUsed = new bool[cities];
	for(int i = 0; i < cities; i++){
		citiesUsed[i] = false;
	}
	int citie;
	double x, y;
	for(int i = 0; i < cities; i++){
		citie = 0;
		x = 0;
		y = 0;
		if(input.eof()){
			ok = false;
		}else{
			citie = this->readNextNumber(input);
			x = this->readNextNumber(input);
			y = this->readNextNumber(input);
		}
		if(citie > 0 && citie <= cities){
			coordinates[citie-1][0] = x;
			coordinates[citie-1][1] = y;
			citiesUsed[citie-1] = true;
		}
	}

	// check if all cities are used
	int i = 0;
	while(ok && i < cities){
		ok = ok && citiesUsed[i];
		i++;
	}
	if(!ok){
		std::cerr << "Error: not al cities loaded" << std::endl;
	}
	return ok;
}

