/*
 * Connection.cpp
 *
 *  Created on: 15/10/2009
 *      Author: jmfu
 */

#include "Connection.h"

Connection::Connection(Neuron* input, Neuron* output, bool recurrent) {
	this->input = input;
	this->output = output;
	this->recurrent = recurrent;
	this->modification = 0;
}

Connection::~Connection() {
}

void Connection::addToModification(double newModification){
	this->modification += newModification;
}

void Connection::applyModification(){
	this->weight += this->modification;
	this->modification = 0;
}

Neuron* Connection::getOuputNeuron(){
	return this->output;
}

Neuron* Connection::getInputNeuron(){
	return this->input;
}

double Connection::getValue(){
	return this->input->getOutput() * this->weight;
}

double Connection::getValue(int delay){
	return this->input->getOutput(delay) * this->weight;
}

void Connection::setWeight(double newWeight){
	this->weight = newWeight;
}

double Connection::getWeight(){
	return this->weight;
}

bool Connection::getRecurrent(){
	return this->recurrent;
}
