#include "TSPFitness.h"
#include "TSPIndividual.h"
#include <iostream>
#include <math.h>


TSPFitness::TSPFitness(double** distanceMatrix, double** coordinatesMatrix, AlgorithmTSP::DATA_TYPE dataType)
{
	this->distanceMatrix = distanceMatrix;
	this->coordinatesMatrix = coordinatesMatrix;
	this->dataType = dataType;
}


TSPFitness::~TSPFitness()
{
}


double TSPFitness::evaluate(Individual* individual)
{	
	double distance = 0;
	int numberOfCities = ((TSPIndividual*)individual)->getNumberCities();
	int* cities = ((TSPIndividual*)individual)->getCitiesRoute();
	if(numberOfCities <= MAX_NUMBER_CITIES_DISTANCE || 
	(dataType != AlgorithmTSP::COORDINATES_EUC_2D && dataType != AlgorithmTSP::COORDINATES_EUC_2D_CEIL &&
	 dataType != AlgorithmTSP::COORDINATES_ATT && dataType != AlgorithmTSP::COORDINATES_GEO_2D)){
		for(int i = 0; i < numberOfCities-1; i++){
			distance += this->distanceMatrix[cities[i]-1][cities[i+1]-1];
		}
		distance += this->distanceMatrix[cities[numberOfCities-1]-1][cities[0]-1];
	}else{
		switch(dataType){
			case AlgorithmTSP::COORDINATES_EUC_2D:
				distance = evaluateEuclideanDistance(cities, numberOfCities);
				break;	
			case AlgorithmTSP::COORDINATES_EUC_2D_CEIL:
				distance = evaluateEuclideanDistanceCeils(cities, numberOfCities);
				break;	
			case AlgorithmTSP::COORDINATES_ATT:
				distance = evaluateATTDistance(cities, numberOfCities);
				break;	
			case AlgorithmTSP::COORDINATES_GEO_2D:
				distance = evaluateGeographicalDistance(cities, numberOfCities);
				break;	
			default:
				std::cerr << "Error: imposible to evaluate an individual" << std::endl;
				break;	
		}
	}
	
	return distance;
}

	
double TSPFitness::evaluateEuclideanDistance(int* cities, int numberOfCities)
{
	double distance = 0;
	double xd, yd;
	for(int i = 0; i < numberOfCities-1; i++){
		xd = this->coordinatesMatrix[cities[i]-1][0] - this->coordinatesMatrix[cities[i+1]-1][0];
		yd = this->coordinatesMatrix[cities[i]-1][1] - this->coordinatesMatrix[cities[i+1]-1][1];
		distance += (int)( sqrt( xd*xd + yd*yd ) );
	}
	xd = this->coordinatesMatrix[cities[numberOfCities-1]-1][0] - this->coordinatesMatrix[cities[0]-1][0];
	yd = this->coordinatesMatrix[cities[numberOfCities-1]-1][1] - this->coordinatesMatrix[cities[0]-1][1];
	distance += (int)( sqrt( xd*xd + yd*yd ) );
	return distance;
}

	
double TSPFitness::evaluateEuclideanDistanceCeils(int* cities, int numberOfCities)
{
	double distance = 0;
	double xd, yd;
	for(int i = 0; i < numberOfCities-1; i++){
		xd = this->coordinatesMatrix[cities[i]-1][0] - this->coordinatesMatrix[cities[i+1]-1][0];
		yd = this->coordinatesMatrix[cities[i]-1][1] - this->coordinatesMatrix[cities[i+1]-1][1];
		distance += ceil( sqrt( xd*xd + yd*yd ));
	}
	xd = this->coordinatesMatrix[cities[numberOfCities-1]-1][0] - this->coordinatesMatrix[cities[0]-1][0];
	yd = this->coordinatesMatrix[cities[numberOfCities-1]-1][1] - this->coordinatesMatrix[cities[0]-1][1];
	distance += ceil( sqrt( xd*xd + yd*yd ));
	return distance;
}
	
	
double TSPFitness::evaluateGeographicalDistance(int* cities, int numberOfCities)
{
	double PI = 3.141592;
	double RRR = 6378.388;
	double distance = 0;
	double latitude1, longitude1, latitude2, longitude2, q1, q2, q3;
	int deg;
	double min;
	
	for(int i = 0; i < numberOfCities-1; i++){		
		deg = (int)this->coordinatesMatrix[cities[i]-1][0];
		min = this->coordinatesMatrix[cities[i]-1][0]-deg;
		latitude1 = PI * (deg + 5.0 * min / 3.0) / 180.0;
		deg = (int)this->coordinatesMatrix[cities[i]-1][1];
		min = this->coordinatesMatrix[cities[i]-1][1]-deg;
		longitude1 = PI * (deg + 5.0 * min / 3.0) / 180.0; 
		
		deg = (int)this->coordinatesMatrix[cities[i+1]-1][0];
		min = this->coordinatesMatrix[cities[i+1]-1][0]-deg;
		latitude2 = PI * (deg + 5.0 * min / 3.0) / 180.0;
		deg = (int)this->coordinatesMatrix[cities[i+1]-1][1];
		min = this->coordinatesMatrix[cities[i+1]-1][1]-deg;
		longitude2 = PI * (deg + 5.0 * min / 3.0) / 180.0; 
		
		q1 = cos(longitude1 - longitude2);
		q2 = cos(latitude1 - latitude2);
		q3 = cos(latitude1 + latitude2);
		
		distance += (int)( RRR * acos(0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0);
	}
	
	deg = (int)this->coordinatesMatrix[cities[numberOfCities-1]-1][0];
	min = this->coordinatesMatrix[cities[numberOfCities-1]-1][0]-deg;
	latitude1 = PI * (deg + 5.0 * min / 3.0) / 180.0;
	deg = (int)this->coordinatesMatrix[cities[numberOfCities-1]-1][1];
	min = this->coordinatesMatrix[cities[numberOfCities-1]-1][1]-deg;
	longitude1 = PI * (deg + 5.0 * min / 3.0) / 180.0; 
	
	deg = (int)this->coordinatesMatrix[cities[0]-1][0];
	min = this->coordinatesMatrix[cities[0]-1][0]-deg;
	latitude2 = PI * (deg + 5.0 * min / 3.0) / 180.0;
	deg = (int)this->coordinatesMatrix[cities[0]-1][1];
	min = this->coordinatesMatrix[cities[0]-1][1]-deg;
	longitude2 = PI * (deg + 5.0 * min / 3.0) / 180.0; 
	
	q1 = cos(longitude1 - longitude2);
	q2 = cos(latitude1 - latitude2);
	q3 = cos(latitude1 + latitude2);
	
	distance += (int)( RRR * acos(0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0);
	
	return distance;
}
	
	
double TSPFitness::evaluateATTDistance(int* cities, int numberOfCities)
{
	double distance = 0;
	double xd, yd, r, t;
	for(int i = 0; i < numberOfCities-1; i++){
		xd = this->coordinatesMatrix[cities[i]-1][0] - this->coordinatesMatrix[cities[i+1]-1][0];
		yd = this->coordinatesMatrix[cities[i]-1][1] - this->coordinatesMatrix[cities[i+1]-1][1];
		r =  sqrt( (xd*xd + yd*yd) / 10.0 );
		t = (int)r;
		if(t<r){
			distance += t+1;	
		}else{
			distance += t;
		}
	}
	xd = this->coordinatesMatrix[cities[numberOfCities-1]-1][0] - this->coordinatesMatrix[cities[0]-1][0];
	yd = this->coordinatesMatrix[cities[numberOfCities-1]-1][1] - this->coordinatesMatrix[cities[0]-1][1];
	r =  sqrt( (xd*xd + yd*yd) / 10.0 );
	t = (int)r;
	if(t<r){
		distance += t+1;	
	}else{
		distance += t;
	}
	return distance;
}
