/*
 * InputNeuron.h
 *
 *  Created on: 15/10/2009
 *      Author: jmfu
 */

#ifndef INPUTNEURON_H_
#define INPUTNEURON_H_

#include "Neuron.h"

class InputNeuron: public Neuron {

public:

	InputNeuron(int id, int maximumDelay);

	virtual ~InputNeuron();

	void setInput(double input);

	void addInputConnection(Connection* connection);

	void calculateOutput();
};

#endif /* INPUTNEURON_H_ */
