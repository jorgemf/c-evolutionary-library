/*
 * NeuralNet.cpp
 *
 *  Created on: 14-may-2009
 *      Author: jorge
 */

#include "NeuralNet.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "mtrand.h"

NeuralNet::NeuralNet(int delay) {
	this->delay = delay;
}

NeuralNet::NeuralNet(int delay, int inputNeurons, int hiddenNeurons,
		int outputNeurons) {
	this->delay = delay;
	createNet(inputNeurons, hiddenNeurons, outputNeurons);
}

NeuralNet::NeuralNet(int delay, int inputNeurons, int layers,
		int* hiddenNeurons, int outputNeurons) {
	this->delay = delay;
	createNet(inputNeurons, layers, hiddenNeurons, outputNeurons);
}

NeuralNet::~NeuralNet() {
}

void NeuralNet::createNet(int inputNeurons, int hiddenNeurons,
		int outputNeurons) {
	// creating neurons
	createNeurons(inputNeurons, hiddenNeurons, outputNeurons);
	// create connections
	int i;
	int j;
	int numberOfNeurons = getNumberOfNeurons();
	for (i = 0; i < numberOfNeurons; i++) {
		for (j = inputNeurons + 1; j < numberOfNeurons; j++) {
			Connection* co = new Connection(this->neurons[i], this->neurons[j],
					j >= i);
			this->neurons[i]->addOutputConnection(co);
			this->neurons[j]->addInputConnection(co);
			this->connections.insert(this->connections.end(), co);
		}
	}
}

void NeuralNet::createNeurons(int inputNeurons, int hiddenNeurons,
		int outputNeurons) {
	int numberOfNeurons = inputNeurons + hiddenNeurons + outputNeurons + 1;
	int ids = 0;
	this->neurons = new Neuron*[numberOfNeurons];
	this->biasNeuron = new BiasNeuron(ids);
	this->neurons[ids] = this->biasNeuron;
	ids++;
	int i;
	for (i = 0; i < inputNeurons; i++) {
		this->neurons[ids] = new InputNeuron(ids, this->delay);
		this->inputNeurons.insert(this->inputNeurons.end(),
				(InputNeuron*) this->neurons[ids]);
		ids++;
	}
	for (i = 0; i < hiddenNeurons; i++) {
		this->neurons[ids] = new Neuron(ids, this->delay);
		this->hiddenNeurons.insert(this->hiddenNeurons.end(),
				this->neurons[ids]);
		ids++;
	}
	for (i = 0; i < outputNeurons; i++) {
		this->neurons[ids] = new Neuron(ids, this->delay);
		this->outputNeurons.insert(this->outputNeurons.end(),
				this->neurons[ids]);
		ids++;
	}
}
void NeuralNet::createNet(int inputNeurons, int layers, int* hiddenNeurons,
		int outputNeurons) {
	// creating neurons
	createNeurons(inputNeurons, layers, hiddenNeurons, outputNeurons);
	// create connections
	int i, j, k;
	int numberOfNeurons = getNumberOfNeurons();

	// inputs with first hidden layer
	int acumulateHiddenL1 = 1;
	int acumulateHiddenL2 = inputNeurons+1;
//	std::cout << "input with first hidden" << std::endl;
	for (i = 0; i < inputNeurons; i++) {
		for (j = 0; j < hiddenNeurons[0]; j++) {
			Connection* co = new Connection(this->neurons[i+acumulateHiddenL1], this->neurons[j+acumulateHiddenL2],	false);
			this->neurons[i+acumulateHiddenL1]->addOutputConnection(co);
			this->neurons[j+acumulateHiddenL2]->addInputConnection(co);
			this->connections.insert(this->connections.end(), co);
//			std::cout << (i+acumulateHiddenL1) << " -> " << (j+acumulateHiddenL2) << std::endl;
		}
	}
//	std::cout << "among hidden layers" << std::endl;
	// hidden layers among them
	for (k = 0; k < layers - 1; k++) {
		acumulateHiddenL1 = acumulateHiddenL2;
		acumulateHiddenL2 += hiddenNeurons[k];
		for (i = 0; i < hiddenNeurons[k]; i++) {
			for (j = 0; j < hiddenNeurons[k + 1]; j++) {
				Connection* co = new Connection(this->neurons[i+acumulateHiddenL1], this->neurons[j+acumulateHiddenL2],	false);
				this->neurons[i+acumulateHiddenL1]->addOutputConnection(co);
				this->neurons[j+acumulateHiddenL2]->addInputConnection(co);
				this->connections.insert(this->connections.end(), co);
//				std::cout << (i+acumulateHiddenL1) << " -> " << (j+acumulateHiddenL2) << std::endl;
			}
		}
	}
//	std::cout << "hidden with output layer" << std::endl;
	// last hidden layer with output layer
	acumulateHiddenL1 = acumulateHiddenL2;
	acumulateHiddenL2 += hiddenNeurons[layers - 1];
	for (i = 0; i < hiddenNeurons[layers - 1]; i++) {
		for (j = 0; j < outputNeurons; j++) {
			Connection* co = new Connection(this->neurons[i+acumulateHiddenL1], this->neurons[j+acumulateHiddenL2],	false);
			this->neurons[i+acumulateHiddenL1]->addOutputConnection(co);
			this->neurons[j+acumulateHiddenL2]->addInputConnection(co);
			this->connections.insert(this->connections.end(), co);
//			std::cout << (i+acumulateHiddenL1) << " -> " << (j+acumulateHiddenL2) << std::endl;

		}
	}
//	std::cout << "bias connections" << std::endl;
	// bias neuron connections
	for (i = inputNeurons+1; i < numberOfNeurons; i++) {
		Connection* co = new Connection(this->neurons[0], this->neurons[i],	false);
		this->neurons[0]->addOutputConnection(co);
		this->neurons[i]->addInputConnection(co);
		this->connections.insert(this->connections.end(), co);
//		std::cout << "0 -> " << i << std::endl;
	}

}

void NeuralNet::createNeurons(int inputNeurons, int layers, int* hiddenNeurons,
		int outputNeurons) {
	int i, j;
	int numberOfNeurons = inputNeurons + outputNeurons + 1;
	for(i = 0; i < layers; i++){
		numberOfNeurons += hiddenNeurons[i];
	}
	int ids = 0;
	this->neurons = new Neuron*[numberOfNeurons];
	this->biasNeuron = new BiasNeuron(ids);
	this->neurons[ids] = this->biasNeuron;
	ids++;
	for (i = 0; i < inputNeurons; i++) {
		this->neurons[ids] = new InputNeuron(ids, this->delay);
		this->inputNeurons.insert(this->inputNeurons.end(),
				(InputNeuron*) this->neurons[ids]);
		ids++;
	}
	for (j = 0; j < layers; j++) {
		for (i = 0; i < hiddenNeurons[j]; i++) {
			this->neurons[ids] = new Neuron(ids, this->delay);
			this->hiddenNeurons.insert(this->hiddenNeurons.end(),
					this->neurons[ids]);
			ids++;
		}
	}
	for (i = 0; i < outputNeurons; i++) {
		this->neurons[ids] = new Neuron(ids, this->delay);
		this->outputNeurons.insert(this->outputNeurons.end(),
				this->neurons[ids]);
		ids++;
	}
}

void NeuralNet::cleanState() {
	int numberOfNeurons = 1 + this->inputNeurons.size()
			+ this->outputNeurons.size() + this->hiddenNeurons.size();
	int i;
	for (i = 0; i < numberOfNeurons; i++) {
		this->neurons[i]->cleanState();
	}
}

bool NeuralNet::loadNet(std::string file) {
	std::ifstream input;
	input.open(file.c_str());
	bool ok = input.good();
	if (ok) {
		// create neurons
		int bias;
		int inputNeurons;
		int hiddenNeurons;
		int outputNeurons;
		input >> bias;
		input >> inputNeurons;
		input >> hiddenNeurons;
		input >> outputNeurons;
		if (bias != 1) {
			std::cerr << "Bias different from 1 not supported" << std::endl;
			input.close();
			return false;
		}
		if (inputNeurons < 0) {
			std::cerr << "Input neurons must be greater or equal 0" << std::endl;
			input.close();
			return false;
		}
		if (hiddenNeurons <= 0) {
			std::cerr << "Hidden neurons must be greater than 0" << std::endl;
			input.close();
			return false;
		}
		if (outputNeurons <= 0) {
			std::cerr << "Output neurons must be greater than 0" << std::endl;
			input.close();
			return false;
		}
		int numberOfConnections;
		input >> numberOfConnections;
		if (numberOfConnections <= 0) {
			std::cerr << "Should be at least one connection" << std::endl;
			input.close();
			return false;
		}
		createNeurons(inputNeurons, hiddenNeurons, outputNeurons);
		int numerOfNeurons = bias + inputNeurons + hiddenNeurons
				+ outputNeurons;
		// create connections
		int i = 0;
		int outputNeuron;
		int inputNeuron;
		double weight;
		for (i = 0; i < numberOfConnections; i++) {
			input >> outputNeuron;
			input >> inputNeuron;
			input >> weight;
			if (outputNeuron < 0 || outputNeuron >= numerOfNeurons
					|| inputNeuron <= inputNeurons || inputNeuron
					>= numerOfNeurons) {
				std::cerr << "Wrong connection check: " << outputNeuron
						<< " -> " << inputNeuron << std::endl;
				input.close();
				return false;
			}
			ok = !input.eof();
			Connection* co = new Connection(this->neurons[outputNeuron],
					this->neurons[inputNeuron], outputNeuron >= inputNeuron);
			co->setWeight(weight);
			this->neurons[outputNeuron]->addOutputConnection(co);
			this->neurons[inputNeuron]->addInputConnection(co);
			this->connections.insert(this->connections.end(), co);
		}
	} else {
		std::cerr << "Input file not opened: " << file << std::endl;
	}
	input.close();
	return ok;
}

bool NeuralNet::saveNet(std::string file) {
	std::ofstream output;
	output.open(file.c_str());
	bool ok = output.good();
	if (ok) {
		output.precision(16);
		// number of neurons (bias,inputs,hidden,outputs)
		output << "1 ";
		output << this->inputNeurons.size() << " ";
		output << this->hiddenNeurons.size() << " ";
		output << this->outputNeurons.size() << " ";
		output << std::endl;
		// number connections
		output << this->connections.size() << " ";
		output << std::endl;
		// connections with weights
		std::vector<Connection*>::iterator it;
		Connection* co;
		for (it = this->connections.begin(); it < this->connections.end(); it++) {
			co = *it;
			output << co->getInputNeuron()->getID() << " ";
			output << co->getOuputNeuron()->getID() << " ";
			output << co->getWeight() << std::endl;
		}
	}
	output.close();
	return ok;
}

void NeuralNet::getOutput(double* input, double* output) {
	int inputs = this->inputNeurons.size();
	int outputs = this->outputNeurons.size();
	int hidden = this->hiddenNeurons.size();
	int bias = 1;
	int total = bias + inputs + outputs + hidden;
	int i;
	InputNeuron* in;
	for (i = 0; i < inputs; i++) {
		in = (InputNeuron*) this->neurons[i + bias];
		in->setInput(input[i]);
	}
	for (i = bias + inputs; i < total; i++) {
		this->neurons[i]->calculateOutput();
	}
	Neuron* out;
	for (i = 0; i < outputs; i++) {
		out = this->neurons[i + bias + inputs + hidden];
		output[i] = out->getOutput();
	}
}

void NeuralNet::randomizeWeights(double min, double max) {
	MTRand_closed rand;
	double diff = max - min;
	Connection* co;
	std::vector<Connection*>::iterator it;
	for (it = this->connections.begin(); it < this->connections.end(); it++) {
		co = *it;
		co->setWeight(rand() * diff + min);
	}
}

int NeuralNet::getDelay() {
	return this->delay;
}

int NeuralNet::getNumberOfNeurons() {
	int inputs = this->inputNeurons.size();
	int outputs = this->outputNeurons.size();
	int hidden = this->hiddenNeurons.size();
	return 1 + inputs + outputs + hidden;
}

Neuron** NeuralNet::getNeurons() {
	return this->neurons;
}

int NeuralNet::getInputSize() {
	return this->inputNeurons.size();
}

int NeuralNet::getOutputSize() {
	return this->outputNeurons.size();
}

std::vector<Connection*> NeuralNet::getConnections() {
	return this->connections;
}
