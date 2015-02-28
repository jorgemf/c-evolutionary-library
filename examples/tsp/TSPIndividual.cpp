#include "TSPIndividual.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>


TSPIndividual::TSPIndividual(int numberOfCities)
{
	this->numberOfCities = numberOfCities;
	if(this->numberOfCities > 0){
		this->cities = new int[this->numberOfCities];
	}
}


TSPIndividual::~TSPIndividual()
{
	if(this->numberOfCities > 0){
		delete this->cities;
	}
}


bool TSPIndividual::saveRouteFile(std::string file, long evaluations)
{
	double fitness = getFitness();
	bool ok = false;
	unsigned int rpos = file.rfind('/');
	if(rpos != std::string::npos && rpos < file.length()){
		std::string dir = file.substr(0,rpos);
		unsigned int lastpos = 0;
		unsigned int pos = file.find('/', lastpos);
		while(pos != std::string::npos && pos < file.length()){
			std::string aux = file.substr(0,pos);
			mkdir(aux.c_str(),0777);
			lastpos = pos+1;
			pos = file.find('/', lastpos);
		}
	}
	std::ofstream output;
	output.open(file.c_str());
	ok = output.good();
	if(ok){
		output << "# Fitness: " << fitness << std::endl;
		output << "# Evaluations: " << evaluations << std::endl;
		output << "# Cities:  " << this->numberOfCities << std::endl;
		for(int i = 0; i < this->getNumberOfObjectives(); i++){
			output << "# Objective " << i << ":" << this->getObjective(i) << std::endl;
		}
		output << "#" << std::endl;
		int* cities = this->getCitiesRoute();
		int numberOfCities = this->getNumberCities();
		for(int i = 0; i < numberOfCities; i++){
			output << cities[i] << std::endl;
		}
	}
	output.close();
	return ok;
}


void TSPIndividual::copy(Individual* individual)
{
	TSPIndividual* individualTSP = (TSPIndividual*)individual;
	if(this->numberOfCities != individualTSP->numberOfCities){
		if(individualTSP->numberOfCities > 0){
			delete individualTSP->cities;
		}
		individualTSP->numberOfCities = this->numberOfCities;
		if(this->numberOfCities > 0){
			individualTSP->cities = new int[this->numberOfCities];
		}
	}
	for(int i = 0; i < this->numberOfCities; i++){
		individualTSP->cities[i] = this->cities[i];
	}
}


int* TSPIndividual::getCitiesRoute()
{
	return this->cities;
}

