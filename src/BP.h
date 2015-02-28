/*
 * BP.h
 *
 *  Created on: 14/01/2010
 *      Author: jorge
 */

#ifndef BP_H_
#define BP_H_

#include <string>
#include "NeuralNet.h"
#include "Data.h"

class BP {

public:

	BP();

	virtual ~BP();

	double learn(NeuralNet* net, Data** data, int numberOfData, Data* validate, Data* test, int cycles, double initalLearning, double finalLearning, std::string netOutputFile);

private:

	double test(NeuralNet* net, Data** data, int numberOfData, double* netOutput);

};

#endif /* BP_H_ */
