/*
 * Util.cpp
 *
 *  Created on: 18-may-2009
 *      Author: jorge
 */

#include "Util.h"
#include <iostream>
#include <sstream>

Util::Util(std::string netOutputFile) {
	this->netOutputFile = netOutputFile;
	this->netOutputFileBestTest = netOutputFile+".bestTest";
}

Util::~Util() {
}

void Util::showData(int iteration, int cycles, double error, double learn, NeuralNet* net, double trainError, double testError, double validateError) {
	int module = cycles / 1000;
	if(iteration == 0){
		this->bestErrorTest = testError;
	}else if(this->bestErrorTest > testError){
		this->bestErrorTest = testError;
		net->saveNet(netOutputFileBestTest);
	}
	if(module == 0 || iteration % module == 0 || iteration == cycles-1){
		double percent = 100.0 * iteration / cycles;
		int units = (int)percent;
		int decimals = (int)(percent * 10) % 10;
		time(&timeEnd);
		double diff = difftime (this->timeEnd,this->timeStart);
		int totalSeconds = (int)diff;
		int seconds = totalSeconds % 60;
		int minutes = (totalSeconds / 60) % 60;
		int hours = (totalSeconds / 3600);
		int totalStimatedSeconds = (int)((1-percent/100)*totalSeconds/(percent/100));
		int secondsLeft = totalStimatedSeconds % 60;
		int minutesLeft = (totalStimatedSeconds / 60) % 60;
		int hoursLeft = (totalStimatedSeconds / 3600);
		std::cout << "#    " << units << "." << decimals << " \t ";
		std::cout << "error: " << error << " \t(learn: " << learn << " )\t time elapse: ";
		std::cout << hours << "h " << minutes << "m " << seconds << "s \t time left: ";
		std::cout << hoursLeft << "h " << minutesLeft << "m " << secondsLeft << "s" << std::endl;
		std::cout << "# trainError " << trainError << " validateError " << validateError << " testError " << testError << std::endl;
		std::cout << iteration << " " <<  error << " " << learn << " " << trainError << " " << validateError << " " << testError << std::endl;
		if(lastSaveNet*10 < percent){
			if(!net->saveNet(netOutputFile)){
				std::cerr << "Net not saved!!!! " << netOutputFile  << std::endl;
			}
			lastSaveNet++;
		}
	}
}

void Util::startLearning() {
	time (&timeStart);
	lastSaveNet = 1;
}
