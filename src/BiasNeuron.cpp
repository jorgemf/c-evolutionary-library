/*
 * BiasNeuron.cpp
 *
 *  Created on: 15/10/2009
 *      Author: jmfu
 */

#include <iostream>
#include "BiasNeuron.h"

BiasNeuron::BiasNeuron(int id) : Neuron(id, 1) {
}

BiasNeuron::~BiasNeuron() {
}

double BiasNeuron::getOutput(){
	return 1.0;
}

double BiasNeuron::getOutput(int delay){
	return 1.0;
}

void BiasNeuron::addInputConnection(Connection* connection) {
	std::cerr << "No input connections are allowed in BiasNeurons" << std::endl;
}
