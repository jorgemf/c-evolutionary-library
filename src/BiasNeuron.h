/*
 * BiasNeuron.h
 *
 *  Created on: 15/10/2009
 *      Author: jmfu
 */

#ifndef BIASNEURON_H_
#define BIASNEURON_H_

#include "Neuron.h"

class BiasNeuron: public Neuron {

public:

	BiasNeuron(int id);

	virtual ~BiasNeuron();

	void addInputConnection(Connection* connection);

	double getOutput();

	double getOutput(int delay);
};

#endif /* BIASNEURON_H_ */
