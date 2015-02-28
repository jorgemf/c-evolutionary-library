/*
 * Neuron.cpp
 *
 *  Created on: 15/10/2009
 *      Author: jmfu
 */

#include <iostream>
#include <cmath>
#include "Neuron.h"

Neuron::Neuron(int id, int maximumDelay) {
	if(maximumDelay <= 0 ){
		std::cerr << "maximumDelay must be greater than 0" << std::endl;
		maximumDelay = 1;
	}
	this->id = id;
	this->outputs = new double[maximumDelay];
	this->maximumDelay = maximumDelay;
	this->cleanState();
}

Neuron::~Neuron() {
	delete this->outputs;
}

void Neuron::addInputConnection(Connection* connection){
	this->inputConnections.insert(this->inputConnections.end(),connection);
}

void Neuron::addOutputConnection(Connection* connection){
	this->outputConnections.insert(this->outputConnections.end(),connection);
}

double Neuron::getOutput(){
	return this->outputs[0];
}

double Neuron::getOutput(int delay){
	if(delay < 0 || delay >= this->maximumDelay){
		std::cerr << "Error while accessing output of neuron, wrong delay" << std::endl;
		delay = 0;
	}
	return this->outputs[delay];
}

void Neuron::calculateOutput(){
	double sum = 0;
	std::vector<Connection*>::iterator it;
	Connection* co;
	for ( it = this->inputConnections.begin() ; it < this->inputConnections.end(); it++ ){
		co = (*it);
		sum += co->getWeight() * co->getInputNeuron()->getOutput();
	}
	// sigmoidal function
	double output = 1.0 / (1.0 + exp(-sum));
	// store the output
	// move outputs and remove the oldest
	int i;
	for(i = this->maximumDelay - 1; i > 0; i--){
		this->outputs[i] = this->outputs[i-1];
	}
	this->outputs[0] = output;
}

void Neuron::cleanState(){
	int i;
	for(i = 0; i < this->maximumDelay; i++){
		this->outputs[i] = 0;
	}
}

int Neuron::getID(){
	return this->id;
}

std::vector<Connection*> Neuron::getInputConnections(){
	return this->inputConnections;
}

std::vector<Connection*> Neuron::getOutputConnections(){
	return this->outputConnections;
}
