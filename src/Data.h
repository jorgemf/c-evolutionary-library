/*
 * Data.h
 *
 *  Created on: 15-may-2009
 *      Author: jorge
 */

#ifndef DATA_H_
#define DATA_H_

#include <string>
#include <vector>

class Data {

public:

	Data();

	Data(double** dataInput, double** dataOutput, int numberOfData, int inputSize, int outputSize);

	virtual ~Data();

	bool loadData(std::string file);

	bool saveData(std::string file);

	inline int getNumberOfData(){
		return this->numberOfData;
	}

	inline int getInputSize(){
		return this->inputSize;
	}

	inline int getOutputSize(){
		return this->outputSize;
	}

	inline double** getDataInput(){
		return this->dataInput;
	}

	inline double** getDataOutput(){
		return this->dataOutput;
	}

	void addData(double* inputs, double* outputs);


private:

	double** dataInput;

	double** dataOutput;

	int numberOfData;

	int inputSize;

	int outputSize;
};

#endif /* DATA_H_ */
