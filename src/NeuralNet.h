/*
 * NeuralNet.h
 *
 *  Created on: 14-may-2009
 *      Author: jorge
 */

#ifndef NEURALNET_H_
#define NEURALNET_H_

#include <string>
#include <vector>
#include "Neuron.h"
#include "Connection.h"
#include "InputNeuron.h"
#include "BiasNeuron.h"

class NeuralNet {

private:

	int delay;

	std::vector<InputNeuron*> inputNeurons;

	std::vector<Neuron*> hiddenNeurons;

	std::vector<Neuron*> outputNeurons;

	BiasNeuron* biasNeuron;

	std::vector<Connection*> connections;

	Neuron** neurons;

	void createNet(int inputNeurons, int hiddenNeurons, int outputNeurons);

	void createNet(int inputNeurons, int layers, int* hiddenNeurons, int outputNeurons);

	void createNeurons(int inputNeurons, int hiddenNeurons, int outputNeurons);

	void createNeurons(int inputNeurons, int layers, int* hiddenNeurons, int outputNeurons);

public:

	NeuralNet(int delay);

	NeuralNet(int delay, int inputNeurons, int hiddenNeurons, int outputNeurons);

	NeuralNet(int delay, int inputNeurons, int layers, int* hiddenNeurons, int outputNeurons);

	virtual ~NeuralNet();

	bool loadNet(std::string file);

	bool saveNet(std::string file);

	//sizes will not be checked here
	void getOutput(double* input, double* output);

	void cleanState();

	void randomizeWeights(double min, double max);

	int getDelay();

	int getNumberOfNeurons();

	Neuron** getNeurons();

	int getInputSize();

	int getOutputSize();

	std::vector<Connection*> getConnections();

};

#endif /* NEURALNET_H_ */
