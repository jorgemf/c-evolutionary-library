/*
 * InputNeuron.cpp
 *
 *  Created on: 15/10/2009
 *      Author: jmfu
 */

#include <iostream>
#include "InputNeuron.h"

InputNeuron::InputNeuron(int id, int maximumDelay) : Neuron(id, maximumDelay) {
}

InputNeuron::~InputNeuron() {
}

void InputNeuron::setInput(double input) {
	// store the output
	// move outputs and remove the oldest
	int i;
	for(i = this->maximumDelay - 1; i > 0; i--){
		this->outputs[i] = this->outputs[i-1];
	}
	this->outputs[0] = input;
}

void InputNeuron::addInputConnection(Connection* connection) {
	std::cerr << "No input connections are allowed in InputNeurons" << std::endl;
}

void InputNeuron::calculateOutput() {
	// do nothing, must perform setInput instead
}
