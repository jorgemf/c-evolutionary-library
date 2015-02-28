/*
 * BPTT.cpp
 *
 *  Created on: 16/10/2009
 *      Author: jmfu
 */

#include <cmath>
#include <iostream>
#include "BPTT.h"
#include "Util.h"

BPTT::BPTT() {
}

BPTT::~BPTT() {
}

double BPTT::learn(NeuralNet* net, Data** data, int numberOfData,
		Data* validate, Data* test, int cycles, double initalLearning,
		double finalLearning, std::string netOutputFile) {
	Util* util = new Util(netOutputFile);
	util->startLearning();
	double error = 0;
	double trainError = 0;
	double testError = 0;
	double validationError = 0;
	double fileError;
	int delay = net->getDelay();
	int numberOfNeurons = net->getNumberOfNeurons();
	Neuron** neurons = net->getNeurons();
	int inputSize = net->getInputSize();
	int outputSize = net->getOutputSize();
	//	int hiddenSize = numberOfNeurons - ( 1 + inputSize + outputSize );
	double** inputData;
	double** outputData;
	double* input;
	double* output;
	double* neuralOutput;
	neuralOutput = new double[outputSize];
	int cycle;
	int datafile;
	int numberOfPatterns;
	int pattern;
	// learning rate
	// learning = a / ( ln(x+1) + 1) + b where x is the actual cycle
	double learning;
	double z = log(cycles + 1) + 1;
	double b = (finalLearning * z - initalLearning) / (z - 1);
	double a = initalLearning - b;
	// for auxiliar operations
	double auxError;
	double aux;
	int delayIndex;

	int offside = numberOfNeurons - outputSize;

	// sigmas (not included the bias neither the inputs)
	double* sigmas = new double[numberOfNeurons];

	// auxiliar index for loops
	int i;

	double neuronOutput;
	std::vector<Connection*> vector;
	std::vector<Connection*>::iterator it;
	Connection* connection;
	int totalNumberOfPatterns = 0;

	for (cycle = 0; cycle < cycles; cycle++) {
		// calcule the actual learning rate
		learning = a / (log(cycle + 1) + 1) + b;
		//sets the error to 0 for each cycle
		error = 0;
		totalNumberOfPatterns = 0;
		for (datafile = 0; datafile < numberOfData; datafile++) {
			// first clean state, each datafile starts with the initial state
			net->cleanState();
			// gets the data from the datafile
			inputData = data[datafile]->getDataInput();
			outputData = data[datafile]->getDataOutput();
			numberOfPatterns = data[datafile]->getNumberOfData();
			totalNumberOfPatterns += numberOfPatterns;

			// browse all patterns for training, starts in delay
			fileError = 0;
			delay = 0;
			for (pattern = delay; pattern < numberOfPatterns; pattern++) {

				// TODO how to start the input??? (now, do not care about that)
				/*				for(delayIndex = delay; delayIndex > 0; delayIndex--){
				 input = inputData[pattern-delayIndex];
				 net->getOutput(input,neuralOutput);
				 }*/
				input = inputData[pattern];
				output = outputData[pattern];
				net->getOutput(input, neuralOutput);

				// sigmas to 0;
				for (i = 0; i < numberOfNeurons; i++) {
					sigmas[i] = 0;
				}

				// sigmas last layer & error of this timestep
				// we are in the first time step, without delay
				auxError = 0;
				for (i = offside; i < numberOfNeurons; i++) {
					neuronOutput = neurons[i]->getOutput(0);
					aux = output[i - offside] - neuronOutput;
					auxError += aux * aux;
					sigmas[i] = (neuronOutput - output[i - offside])
							* neuronOutput * (1 - neuronOutput);
				}
				// increase error
				fileError += auxError / outputSize;

				// sigma other layers (from last to second, first are the inputs)
				// we are still in the first time step, without delay
				for (i = offside - 1; i > inputSize; i--) {
					neuronOutput = neurons[i]->getOutput(0);
					aux = 0;
					// browse all no recurrent output connections
					vector = neurons[i]->getOutputConnections();
					for (it = vector.begin(); it < vector.end(); it++) {
						connection = (Connection*) (*it);
						if (!connection->getRecurrent()) {
							aux
									+= sigmas[connection->getOuputNeuron()->getID()]
											* connection->getWeight();
						}
					}
					sigmas[i] = neuronOutput * (1 - neuronOutput) * aux;
				}

				// add the update to the connections (all the connections, this is, all input connections of the neurons except bias and inputs)
				for (i = inputSize + 1; i < numberOfNeurons; i++) {
					vector = neurons[i]->getInputConnections();
					for (it = vector.begin(); it < vector.end(); it++) {
						connection = (Connection*) (*it);
						if (connection->getRecurrent()) {
							neuronOutput
									= connection->getInputNeuron()->getOutput(1);
						} else {
							neuronOutput
									= connection->getInputNeuron()->getOutput(0);
						}
						connection->addToModification(-learning * sigmas[i]
								* neuronOutput);
					}
				}

				// first time step finished, now we do all minus the last one (we suppose the delay is greater that 1
				for (delayIndex = 1; delayIndex < delay - 1; delayIndex++) {

					// sigma all layers
					for (i = numberOfNeurons - 1; i > inputSize; i--) {
						neuronOutput = neurons[i]->getOutput(delayIndex);
						aux = 0;
						// browse all output connections, delay connections should increase its delay
						vector = neurons[i]->getOutputConnections();
						for (it = vector.begin(); it < vector.end(); it++) {
							connection = (Connection*) (*it);
							aux
									+= sigmas[connection->getOuputNeuron()->getID()]
											* connection->getWeight();
						}
						sigmas[i] = neuronOutput * (1 - neuronOutput) * aux;
					}

					// add the update to the connections (all the connections, this is, all input connections of the neurons except bias and inputs)
					for (i = inputSize + 1; i < numberOfNeurons; i++) {
						vector = neurons[i]->getInputConnections();
						for (it = vector.begin(); it < vector.end(); it++) {
							connection = (Connection*) (*it);
							if (connection->getRecurrent()) {
								neuronOutput
										= connection->getInputNeuron()->getOutput(
												delayIndex + 1);
							} else {
								neuronOutput
										= connection->getInputNeuron()->getOutput(
												delayIndex);
							}
							connection->addToModification(-learning * sigmas[i]
									* neuronOutput);
						}
					}
				}

				// last step

				// sigma all layers
				for (i = numberOfNeurons - 1; i > inputSize; i--) {
					neuronOutput = neurons[i]->getOutput(delayIndex);
					aux = 0;
					// browse all output connections, delay connections should increase its delay
					vector = neurons[i]->getOutputConnections();
					for (it = vector.begin(); it < vector.end(); it++) {
						connection = (Connection*) (*it);
						aux += sigmas[connection->getOuputNeuron()->getID()]
								* connection->getWeight();
					}
					sigmas[i] = neuronOutput * (1 - neuronOutput) * aux;
				}

				// add the update to the connections (all the connections, this is, all input connections of the neurons except bias and inputs)
				for (i = inputSize + 1; i < numberOfNeurons; i++) {
					vector = neurons[i]->getInputConnections();
					for (it = vector.begin(); it < vector.end(); it++) {
						connection = (Connection*) (*it);
						if (connection->getRecurrent()) {
							neuronOutput = 0;
						} else {
							neuronOutput
									= connection->getInputNeuron()->getOutput(
											delayIndex);
						}
						connection->addToModification(-learning * sigmas[i]
								* neuronOutput);
					}
				}

				// now update the weighs
				vector = net->getConnections();
				for (it = vector.begin(); it < vector.end(); it++) {
					connection = (Connection*) (*it);
					connection->applyModification();
				}
			}
			error += fileError;
		}
		error /= totalNumberOfPatterns;
		trainError = this->test(net, data, numberOfData, neuralOutput);
		testError = this->test(net, &test, 1, neuralOutput);
		validationError = this->test(net, &validate, 1, neuralOutput);
		util->showData(cycle, cycles, error, learning, net, trainError, testError, validationError);
	}
	// free memory
	delete util;
	delete neuralOutput;
	delete sigmas;

	return error;
}

double BPTT::test(NeuralNet* net, Data** data, int numberOfData,
		double* netOutput) {
	double error = 0;
	int datafile;
	int totalPatterns = 0;
	double* output;
	double* input;
	double** inputData;
	double** outputData;
	double auxError;
	double diff;
	int i, j;
	int netOutputSize = net->getOutputSize();
	/**************************
	int neat_totalOk = 0;
	int neat_ok = 0;
	int neat_bad = 0;
	**************************/
	for (datafile = 0; datafile < numberOfData; datafile++) {
		inputData = data[datafile]->getDataInput();
		outputData = data[datafile]->getDataOutput();
		net->cleanState();
		totalPatterns += data[datafile]->getNumberOfData();
		for (i = 0; i < data[datafile]->getNumberOfData(); i++) {
			input = inputData[i];
			output = outputData[i];
			net->getOutput(input, netOutput);
			auxError = 0;
			for (j = 0; j < net->getOutputSize(); j++) {
				diff = output[j] - netOutput[j];
				auxError += diff * diff;
				/**************************
				if (output[j] > 0.5) {
					neat_totalOk++;
				}
				if (output[j] > 0.5 && netOutput[j] > 0.5) {
					neat_ok++;
				} else if (output[j] > 0.5 || netOutput[j] > 0.5) {
					neat_bad++;
				}
				**************************/
			}
			/**************************
			if(output[0] == 1){
				neat_totalOk++;
				if(netOutput[0] > netOutput[1]){
					neat_ok++;
				}else{
					neat_bad++;
				}
			}else{
				if(netOutput[1] < netOutput[0]){
					neat_bad++;
				}
			}
			**************************/
			error += auxError / netOutputSize;
		}
	}
	/**************************
	netOutputSize = 1;
	**************************
	double neat_rate = 0;
	neat_rate = (double) (neat_ok - neat_bad) / neat_totalOk;
	std::cout << "#    rate: " << neat_rate;
	neat_rate = (double) (totalPatterns * netOutputSize - neat_bad)
			/ (totalPatterns * netOutputSize);
	std::cout << "    total: " << neat_rate << std::endl;
	**************************/
	if (totalPatterns > 0) {
		error /= totalPatterns;
	} else {
		error = -1;
	}
	return error;
}

double BPTT::learn(NeuralNet* net, Data* data, int test, int validate,
		int cycles, double initalLearning, double finalLearning,
		std::string netOutputFile) {
	Util* util = new Util(netOutputFile);
	util->startLearning();
	double error = 0;
	double trainError = 0;
	double testError = 0;
	double validationError = 0;
	double fileError;
	int delay = net->getDelay();
	int numberOfNeurons = net->getNumberOfNeurons();
	Neuron** neurons = net->getNeurons();
	int inputSize = net->getInputSize();
	int outputSize = net->getOutputSize();
	double** inputData;
	double** outputData;
	double* input;
	double* inputValidate = new double[inputSize];
	double* output;
	double* neuralOutput;
	neuralOutput = new double[outputSize];
	int cycle;
	int numberOfPatterns;
	int pattern;
	// learning rate
	// learning = a / ( ln(x+1) + 1) + b where x is the actual cycle
	double learning;
	double z = log(cycles + 1) + 1;
	double b = (finalLearning * z - initalLearning) / (z - 1);
	double a = initalLearning - b;
	// for auxiliar operations
	double auxError;
	double aux;
	int delayIndex;

	int offside = numberOfNeurons - outputSize;

	// sigmas (not included the bias neither the inputs)
	double* sigmas = new double[numberOfNeurons];

	// auxiliar index for loops
	int i;

	double neuronOutput;
	std::vector<Connection*> vector;
	std::vector<Connection*>::iterator it;
	Connection* connection;

	int trainingSamples;

	// gets the data from the datafile
	inputData = data->getDataInput();
	outputData = data->getDataOutput();
	numberOfPatterns = data->getNumberOfData();
	for (cycle = 0; cycle < cycles; cycle++) {
		// calcule the actual learning rate
		learning = a / (log(cycle + 1) + 1) + b;
		//sets the error to 0 for each cycle
		error = 0;
		// first clean state, each datafile starts with the initial state
		net->cleanState();

		// browse all patterns for training, starts in delay
		fileError = 0;
		delay = 0;

		// train
		trainingSamples = numberOfPatterns - (test + validate);
		for (pattern = delay; pattern < trainingSamples; pattern++) {

			input = inputData[pattern];
			output = outputData[pattern];
			net->getOutput(input, neuralOutput);

			// sigmas to 0;
			for (i = 0; i < numberOfNeurons; i++) {
				sigmas[i] = 0;
			}

			// sigmas last layer & error of this timestep
			// we are in the first time step, without delay
			auxError = 0;
			for (i = offside; i < numberOfNeurons; i++) {
				neuronOutput = neurons[i]->getOutput(0);
				aux = output[i - offside] - neuronOutput;
				auxError += aux * aux;
				sigmas[i] = (neuronOutput - output[i - offside]) * neuronOutput
						* (1 - neuronOutput);
			}
			// increase error
			fileError += auxError / outputSize;

			// sigma other layers (from last to second, first are the inputs)
			// we are still in the first time step, without delay
			for (i = offside - 1; i > inputSize; i--) {
				neuronOutput = neurons[i]->getOutput(0);
				aux = 0;
				// browse all no recurrent output connections
				vector = neurons[i]->getOutputConnections();
				for (it = vector.begin(); it < vector.end(); it++) {
					connection = (Connection*) (*it);
					if (!connection->getRecurrent()) {
						aux += sigmas[connection->getOuputNeuron()->getID()]
								* connection->getWeight();
					}
				}
				sigmas[i] = neuronOutput * (1 - neuronOutput) * aux;
			}

			// add the update to the connections (all the connections, this is, all input connections of the neurons except bias and inputs)
			for (i = inputSize + 1; i < numberOfNeurons; i++) {
				vector = neurons[i]->getInputConnections();
				for (it = vector.begin(); it < vector.end(); it++) {
					connection = (Connection*) (*it);
					if (connection->getRecurrent()) {
						neuronOutput = connection->getInputNeuron()->getOutput(
								1);
					} else {
						neuronOutput = connection->getInputNeuron()->getOutput(
								0);
					}
					connection->addToModification(-learning * sigmas[i]
							* neuronOutput);
				}
			}

			// first time step finished, now we do all minus the last one (we suppose the delay is greater that 1
			for (delayIndex = 1; delayIndex < delay - 1; delayIndex++) {

				// sigma all layers
				for (i = numberOfNeurons - 1; i > inputSize; i--) {
					neuronOutput = neurons[i]->getOutput(delayIndex);
					aux = 0;
					// browse all output connections, delay connections should increase its delay
					vector = neurons[i]->getOutputConnections();
					for (it = vector.begin(); it < vector.end(); it++) {
						connection = (Connection*) (*it);
						aux += sigmas[connection->getOuputNeuron()->getID()]
								* connection->getWeight();
					}
					sigmas[i] = neuronOutput * (1 - neuronOutput) * aux;
				}

				// add the update to the connections (all the connections, this is, all input connections of the neurons except bias and inputs)
				for (i = inputSize + 1; i < numberOfNeurons; i++) {
					vector = neurons[i]->getInputConnections();
					for (it = vector.begin(); it < vector.end(); it++) {
						connection = (Connection*) (*it);
						if (connection->getRecurrent()) {
							neuronOutput
									= connection->getInputNeuron()->getOutput(
											delayIndex + 1);
						} else {
							neuronOutput
									= connection->getInputNeuron()->getOutput(
											delayIndex);
						}
						connection->addToModification(-learning * sigmas[i]
								* neuronOutput);
					}
				}
			}

			// last step

			// sigma all layers
			for (i = numberOfNeurons - 1; i > inputSize; i--) {
				neuronOutput = neurons[i]->getOutput(delayIndex);
				aux = 0;
				// browse all output connections, delay connections should increase its delay
				vector = neurons[i]->getOutputConnections();
				for (it = vector.begin(); it < vector.end(); it++) {
					connection = (Connection*) (*it);
					aux += sigmas[connection->getOuputNeuron()->getID()]
							* connection->getWeight();
				}
				sigmas[i] = neuronOutput * (1 - neuronOutput) * aux;
			}

			// add the update to the connections (all the connections, this is, all input connections of the neurons except bias and inputs)
			for (i = inputSize + 1; i < numberOfNeurons; i++) {
				vector = neurons[i]->getInputConnections();
				for (it = vector.begin(); it < vector.end(); it++) {
					connection = (Connection*) (*it);
					if (connection->getRecurrent()) {
						neuronOutput = 0;
					} else {
						neuronOutput = connection->getInputNeuron()->getOutput(
								delayIndex);
					}
					connection->addToModification(-learning * sigmas[i]
							* neuronOutput);
				}
			}

			// now update the weighs
			vector = net->getConnections();
			for (it = vector.begin(); it < vector.end(); it++) {
				connection = (Connection*) (*it);
				connection->applyModification();
			}
		}
		error += fileError/trainingSamples;

		// test
		fileError = 0;
		for (pattern = trainingSamples; pattern < trainingSamples+test; pattern++) {
			input = inputData[pattern];
			if(inputSize == outputSize){
				input = inputValidate;
				for(i = 0; i < inputSize; i++){
					inputValidate[i] = neuralOutput[i];
				}
			}
			output = outputData[pattern];
			net->getOutput(input, neuralOutput);
			auxError = 0;
			for (i = offside; i < numberOfNeurons; i++) {
				neuronOutput = neurons[i]->getOutput(0);
				aux = output[i - offside] - neuronOutput;
				auxError += aux * aux;
			}
			// increase error
			fileError += auxError / outputSize;
		}
		testError = fileError/test;

		// validate
		fileError = 0;
		for (pattern = trainingSamples+test; pattern < trainingSamples+test+validate; pattern++) {
			input = inputData[pattern];
			if(inputSize == outputSize){
				input = inputValidate;
				for(i = 0; i < inputSize; i++){
					inputValidate[i] = neuralOutput[i];
				}
			}
			output = outputData[pattern];
			net->getOutput(input, neuralOutput);
			auxError = 0;
			for (i = offside; i < numberOfNeurons; i++) {
				neuronOutput = neurons[i]->getOutput(0);
				aux = output[i - offside] - neuronOutput;
				auxError += aux * aux;
			}
			// increase error
			fileError += auxError / outputSize;
		}
		validationError = fileError/validate;

		trainError = -1;
		util->showData(cycle, cycles, error, learning, net, trainError, testError, validationError);
	}
	// free memory
	delete inputValidate;
	delete util;
	delete neuralOutput;
	delete sigmas;

	return error;
}

