/*
 * Connection.h
 *
 *  Created on: 15/10/2009
 *      Author: jmfu
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

class Neuron;
#include "Neuron.h"

class Connection {

private:

	double modification;

	double weight;

	Neuron* input;

	Neuron* output;

	bool recurrent;

public:

	Connection(Neuron* input, Neuron* output, bool recurrent);

	virtual ~Connection();

	void addToModification(double newModification);

	void applyModification();

	Neuron* getOuputNeuron();

	Neuron* getInputNeuron();

	double getValue();

	double getValue(int delay);

	void setWeight(double newWeight);

	double getWeight();

	bool getRecurrent();
};

#endif /* CONNECTION_H_ */
