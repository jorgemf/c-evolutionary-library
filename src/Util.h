/*
 * Util.h
 *
 *  Created on: 18-may-2009
 *      Author: jorge
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <time.h>
#include <string>
#include "NeuralNet.h"

class Util {

public:

	Util(std::string netOutputFile);

	virtual ~Util();

	void showData(int iteration, int cycles, double error, double learn, NeuralNet* net, double trainError, double testError, double validateError);

	void startLearning();

private:

	time_t timeStart;

	time_t timeEnd;

	std::string netOutputFile;

	int lastSaveNet;

	double bestErrorTest;

	std::string  netOutputFileBestTest;

};

#endif /* UTIL_H_ */
