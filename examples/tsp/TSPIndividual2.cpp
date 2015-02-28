#include "TSPIndividual2.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>


TSPIndividual2::TSPIndividual2(int numberOfCities) : TSPIndividual (numberOfCities)
{
	this->numberOfCities = numberOfCities;
	if(this->numberOfCities > 0){
		this->citiesDouble = new double[this->numberOfCities];
		this->auxiliar = new double[this->numberOfCities];
	}
}


TSPIndividual2::~TSPIndividual2()
{
	if(this->numberOfCities > 0){
		delete this->citiesDouble;
		delete this->auxiliar;
	}
}
	

void TSPIndividual2::copy(Individual* individual)
{
	TSPIndividual2* individualTSP = (TSPIndividual2*)individual;
	if(this->numberOfCities != individualTSP->numberOfCities){
		if(individualTSP->numberOfCities > 0){
			delete individualTSP->cities;
			delete individualTSP->citiesDouble;
			delete individualTSP->auxiliar;
		}
		individualTSP->numberOfCities = this->numberOfCities;
		if(this->numberOfCities > 0){
			individualTSP->cities = new int[this->numberOfCities];
			individualTSP->citiesDouble = new double[this->numberOfCities];
			individualTSP->auxiliar = new double[this->numberOfCities];
		}
	}
	for(int i = 0; i < this->numberOfCities; i++){
		individualTSP->citiesDouble[i] = this->citiesDouble[i];	
	}
}


int* TSPIndividual2::getCitiesRoute()
{
	// decode the cities in random keys
	
	// copy the double array into auxiliary
	for(int i = 0; i < this->numberOfCities; i++){
		this->auxiliar[i] = this->citiesDouble[i];
	}
	// sorts the auxiliarity array
	quickSort(auxiliar,0,this->numberOfCities-1);
	
	// search the double value in the auxiliarity array to know the citi number
	for(int i = 0; i < this->numberOfCities; i++){
		int index = binarySearch(this->auxiliar, this->citiesDouble[i], 0, this->numberOfCities -1);
		this->cities[i] = index+1; // the first city is the number 1
	}
		
	return this->cities;	
}


void TSPIndividual2::quickSort(double* array, int begin, int end){
	
	if(end > begin) {
		
		// select pivot
		double pivot = array[(begin+end)/2];
		
		// partition
		int left = begin, right = end;
		do{
			while( left <= end && array[left] < pivot){
				left++;	
			}
			while( right >= begin && array[right] > pivot){
				right--;	
			}
			if(left < right){
				double aux;
				aux = array[left];
				array[left] = array[right];
				array[right] = aux;
			}
		}while(left < right);
		
		// recursive sort
		quickSort(array,begin,right-1);
		quickSort(array,left+1,end);
	}
}


int TSPIndividual2::binarySearch(double* array, double searchValue, int begin, int end)
{
	int index = -1;
	while(index == -1 && begin <= end){
		int pos = (begin+end)/2;
		if(array[pos] == searchValue){
			index = pos;
		}else if(array[pos] > searchValue){
			end = pos - 1;
		}else{ // searchValue is bigger than array[pos]
			begin = pos + 1;
		}
	}
	return index;
}

