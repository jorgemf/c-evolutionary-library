/*
 * BPTT.h
 *
 *  Created on: 16/10/2009
 *      Author: jmfu
 */

#ifndef BPTT_H_
#define BPTT_H_

#include <string>
#include "NeuralNet.h"
#include "Data.h"

class BPTT {

public:

	BPTT();

	virtual ~BPTT();

	double learn(NeuralNet* net, Data** data, int numberOfData, Data* validate, Data* test, int cycles, double initalLearning, double finalLearning, std::string netOutputFile);

	double learn(NeuralNet* net, Data* data, int test, int validate, int cycles, double initalLearning, double finalLearning, std::string netOutputFile);

private:

	double test(NeuralNet* net, Data** data, int numberOfData, double* netOutput);

};

#endif /* BPTT_H_ */
