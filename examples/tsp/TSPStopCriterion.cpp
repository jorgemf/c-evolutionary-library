#include "TSPStopCriterion.h"
#include "PropertiesMap.h"
#include <stdlib.h>

TSPStopCriterion::TSPStopCriterion()
{
	maxEvaluaions = 0;
	std::string aux;
	PropertiesMap* properties;
	properties = PropertiesMap::instance();

	aux = properties->get("evaluations");
	if(aux.length() > 0){
		char* finalPtr = 0;
		maxEvaluaions = strtol(aux.c_str(),&finalPtr, 10);
	}

}

TSPStopCriterion::~TSPStopCriterion()
{
}

bool TSPStopCriterion::stop(int generation, Individual **population, int populationSize, long evaluations)
{
	return evaluations < maxEvaluaions;
}
