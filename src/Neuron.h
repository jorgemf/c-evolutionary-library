/*
 * Neuron.h
 *
 *  Created on: 15/10/2009
 *      Author: jmfu
 */

#ifndef NEURON_H_
#define NEURON_H_

#include <vector>

class Connection;
#include "Connection.h"


class Neuron {

private:

	int id;

	std::vector<Connection*> inputConnections;

	std::vector<Connection*> outputConnections;

protected:

	int maximumDelay;

	double* outputs;

public:

	Neuron(int id, int maximumDelay);

	virtual ~Neuron();

	virtual void addInputConnection(Connection* connection);

	virtual void addOutputConnection(Connection* connection);

	virtual double getOutput();

	virtual double getOutput(int delay);

	virtual void calculateOutput();

	virtual void cleanState();

	std::vector<Connection*> getInputConnections();

	std::vector<Connection*> getOutputConnections();

	int getID();

};

#endif /* NEURON_H_ */
