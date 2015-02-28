/*
 * Data.cpp
 *
 *  Created on: 15-may-2009
 *      Author: jorge
 */

#include "Data.h"
#include <iostream>
#include <fstream>
#include "mtrand.h"

Data::Data() {
	this->inputSize = 0;
	this->outputSize = 0;
	this->dataInput = NULL;
	this->dataOutput = NULL;
	this->numberOfData = 0;
}

Data::Data(double** dataInput, double** dataOutput, int numberOfData, int inputSize, int outputSize){
	this->inputSize = inputSize;
	this->outputSize = outputSize;
	this->dataInput = dataInput;
	this->dataOutput = dataOutput;
	this->numberOfData = numberOfData;
}

Data::~Data() {
	if(this->numberOfData > 0){
		for(int i = 0; i < this->numberOfData; i++){
			delete this->dataInput[i];
			delete this->dataOutput[i];
		}
		delete this->dataInput;
		delete this->dataOutput;
		this->numberOfData = 0;
	}
}

bool Data::loadData(std::string file) {
	// number of patterns, number of inputs, number of outputs.

	std::ifstream input;
	input.open(file.c_str());
	bool ok = input.good();
	if(ok){
		int patterns;
		int inputs;
		int outputs;
		input >> patterns;
		input >> inputs;
		input >> outputs;
		if(patterns <= 0 || inputs < 0 || outputs <= 0){
			std::cerr << "Bad file format (patterns, inputs or outputs <=0)" << std::endl;
			ok = false;
		}else{
			int seek = 0;
			if(this->numberOfData > 0){
				if(this->inputSize == inputs && this->outputSize == outputs){
					double** auxInputs = new double*[this->numberOfData + patterns];
					double** auxOutputs = new double*[this->numberOfData + patterns];
					for(int i = 0; i < this->numberOfData; i++){
						auxInputs[i] = this->dataInput[i];
						auxOutputs[i] = this->dataOutput[i];
					}
					for(int i = this->numberOfData; i < this->numberOfData + patterns; i++){
						auxInputs[i] = new double[this->inputSize];
						auxOutputs[i] = new double[this->outputSize];
					}
					delete this->dataInput;
					delete this->dataOutput;
					this->dataInput = auxInputs;
					this->dataOutput = auxOutputs;
					seek = this->numberOfData;
					this->numberOfData = this->numberOfData + patterns;
				}else{
					std::cerr << "File not loaded: number of inputs or outpus different" << std::endl;
					ok = false;
				}
			}else{
				this->numberOfData = patterns;
				this->inputSize = inputs;
				this->outputSize = outputs;
				this->dataInput = new double*[this->numberOfData];
				this->dataOutput = new double*[this->numberOfData];
				for(int i = 0; i < this->numberOfData; i++){
					if(this->inputSize > 0){
						this->dataInput[i] = new double[this->inputSize];
					}
					this->dataOutput[i] = new double[this->outputSize];
				}
			}
			if(ok){
				// read data and store it
				for(int i = seek; i < this->numberOfData; i++){
					for(int j = 0; j < this->inputSize; j++){
						input >> this->dataInput[i][j];
					}
					for(int j = 0; j < this->outputSize; j++){
						input >> this->dataOutput[i][j];
					}
				}
			}
		}
	}else{
		std::cerr << "Input file not opened: " << file << std::endl;
	}
	input.close();
	return ok;
}

bool Data::saveData(std::string file) {
	// number of patterns, number of inputs, number of outputs.

	std::ofstream output;
	output.open(file.c_str());
	bool ok = output.good();
	if(ok){
		if(this->numberOfData > 0){
			output.precision(16);
			output << this->numberOfData << " ";
			output << this->inputSize << " ";
			output << this->outputSize << std::endl;
			// save data
			for(int i = 0; i < this->numberOfData; i++){
				for(int j = 0; j < this->inputSize; j++){
					output << this->dataInput[i][j] << " ";
				}
				output << std::endl;
				for(int j = 0; j < this->outputSize; j++){
					output << this->dataOutput[i][j] << " ";
				}
				output << std::endl;
			}
		}else{
			std::cerr << "Empty data" << std::endl;
		}
	}else{
		std::cerr << "Output file not opened: " << file << std::endl;
	}
	output.close();
	return ok;
}
