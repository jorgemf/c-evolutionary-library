#include "Algorithm.h"
#include <iostream>

Algorithm::Algorithm()
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::quickSort(Individual** population, int begin, int end){
	
	if(end > begin) {
		
		// select pivot
		Individual* pivot = population[(begin+end)/2];
		
		// partition
		int left = begin, right = end;
		do{
			while( left <= end && population[left]->getFitness() <= pivot->getFitness()){
				left++;	
			}
			while( right >= begin && population[right]->getFitness() > pivot->getFitness()){
				right--;	
			}
			if(left < right){
				Individual* aux;
				aux = population[left];
				population[left] = population[right];
				population[right] = aux;
			}
		}while(left < right);
		
		// put pivots at the end because they are the greatest ones in this part
		for(int i = begin; i < right; i++){
			if(population[i]->getFitness() == pivot->getFitness()){
				Individual* aux;
				aux = population[i];
				population[i] = population[right];
				population[right] = aux;
				right--;
			}
		}
		
		// recursive sort
		quickSort(population,begin,right);
		quickSort(population,left,end);
	}
}



void Algorithm::initialize()
{
	
}

void Algorithm::preload()
{
	
}

void Algorithm::endGeneration()
{
	
}

void Algorithm::endAlgorithm()
{
	
}

Individual* Algorithm::createRandomIndividual(int objectives)
{
	return NULL;
}

Individual* Algorithm::createEmptyIndividual(int objectives)
{
	return NULL;
}

void Algorithm::run()
{
	
}

