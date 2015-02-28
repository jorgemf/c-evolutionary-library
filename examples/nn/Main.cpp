/*
 * Main.cpp
 *
 *  Created on: 14-may-2009
 *      Author: jorge
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

#include "mtrand.h"
#include "NeuralNet.h"
#include "Data.h"
#include "LearningBackpropagation.h"
#include "LearningRecurrentBackpropagation.h"
#include "LearningTest.h"

/**
 * Prints default usage of the program.
 */
void usage(){
	cout << "Usage: rNN algorithm algorithmProperties*" << endl << endl;
	cout << "\t algorithm: " << endl;
	cout << "\t         weka       create weka datafiles" << endl;
	cout << "\t       fusion       fusion data files into a one file" << endl;
	cout << "\t   dataFilter       filters data files" << endl;
	cout << "\t         test       test a network with a data file" << endl;
	cout << "\t      analyze       analyze a data file" << endl;
	cout << "\t           lT       a test learning algorithm" << endl;
	cout << "\t            B       backpropagation" << endl;
	cout << "\t           rB       recurrent backpropagation" << endl;
}

void generateRandomSeed(){
	unsigned long int seed = 0;
	if(seed == 0){
		cout << "Generating random seed..." << endl;
		seed = time(NULL);
		std::stringstream buf;
		buf << seed;
		string seedstring = buf.str();
		cout << seedstring << endl;
	}
	MTRand_closed rand(seed);
}

bool getInt(const char* number, int* value){;
	  char* pEnd;
	  long int auxValue = strtol(number,&pEnd,10);
	  *value = (int)auxValue;
	  return *pEnd == '\0';
}

bool getDouble(const char* number, double* value){;
	  char* pEnd;
	  double auxValue = strtod(number,&pEnd);
	  *value = (double)auxValue;
	  return *pEnd == '\0';
}

int loadNetDescription(int* layers, int** neuronsPerlayer, const char* description){
	int neurons = 0;
	int auxLayers = 1;
	int index = 0;
	while(description[index] != '\0'){
		char character = description[index];
		if(character == ':'){
			auxLayers++;
		}
		if(character < '0' && character > '9' && character != ':'){
			return 0;
		}
		index++;
	}
	*layers = auxLayers;
	int* auxNeuronsPerlayer = new int[auxLayers];
	*neuronsPerlayer = auxNeuronsPerlayer;

	char* pEnd;
	char* pStart = (char*)description;
	for(int i = 0; i < auxLayers; i++){
		auxNeuronsPerlayer[i] = strtol(pStart,&pEnd,10);
		pStart = pEnd + sizeof(char);
		neurons += auxNeuronsPerlayer[i];
		if(auxNeuronsPerlayer[i] <= 0){
			return 0;
		}
	}
	return neurons;
}

void usageBackpropagation(){
	cout << "Backpropagation Options" << endl << endl;
	cout << "rNN B neuralNet neuralDescripion initialLearning finalLearning cicles outputNet datafile+" << endl << endl;
	cout << "\t neuralNet            the input neuralnet file. \"none\" to create a new one" << endl;
	cout << "\t neuralDescripion     the number of neurons of each layer separete by \":\". Example 10:20:3" << endl;
	cout << "\t initialLearning      initial learning rate. Range: [0,1]" << endl;
	cout << "\t finalLearning        final learning rate. Range: [0,1] and <= initialLearning" << endl;
	cout << "\t cicles               number of cicles. > 0" << endl;
	cout << "\t outputNet            filename for output net" << endl;
	cout << "\t datafile             datafiles (almost one)" << endl;
}

void backpropagation(int argc, const char* argv[]){

	if(argc < 9){
		cerr << "Wrong arguments" << endl << endl;
		usageBackpropagation();
		return;
	}

	generateRandomSeed();

	//Network
	NeuralNet* net;
	net = new NeuralNet();

	string file(argv[2]);
	if(file != "none"){
		if(!net->loadNet(file)){
			cerr << "Net not loaded " << file  << endl;
			return;
		}
	}

	// net description
	int layers;
	int* neuronsPerLayer;
	int totalNeurons = loadNetDescription(&layers,&neuronsPerLayer, argv[3]);
	if(totalNeurons <= 0){
		cerr << "Neural description wrong" << endl;
		return;
	}
	if(layers < 3){
		cerr << "Wrong number of layers (>=3): "<< layers << endl;
		return;
	}

	if(file == "none"){
		net->createNet(totalNeurons);
		net->randomizeWeights(-1,1);
	}

	double initialLearning ;
	if(!getDouble(argv[4],&initialLearning) || initialLearning < 0 || initialLearning > 1){
		cerr << "Wrong initial learning" << endl;
		return;
	}

	double finalLearning;
	if(!getDouble(argv[5],&finalLearning) || finalLearning < 0 || finalLearning > initialLearning){
		cerr << "Wrong final learning" << endl;
		return;
	}

	int cycles ;
	if(!getInt(argv[6],&cycles) || cycles < 0){
		cerr << "Wrong number of cycles" << endl;
		return;
	}

	string outputNet(argv[7]);

	// data files
	int index = 8;
	Data* data = new Data();
	while(index < argc){
		string dataString(argv[index]);
		if(!data->loadData(dataString)){
			cerr << "Data file not loaded: " << dataString << endl;
			return;
		}else{
			cout << "Loaded: " << dataString << endl;
		}
		index++;
	}

	if(data->getInputSize() != neuronsPerLayer[0]){
		cerr << "Different input size in data and net: " << data->getInputSize() << " " << neuronsPerLayer[0] << endl;
		return;
	}

	if(data->getOutputSize() != neuronsPerLayer[layers-1]){
		cerr << "Different input size in data and net: " << data->getOutputSize() << " " << neuronsPerLayer[layers-1] << endl;
		return;
	}

	if(data->getInputSize() + data->getOutputSize() >= net->getNumberOfNeurons()){
		cerr << "Data input size + output size <= number of neurons of the net" << endl;
		return;
	}

	// neural training
	net->cleanState();
	LearningBackpropagation* learn = new LearningBackpropagation(net,neuronsPerLayer,layers);
	double error = learn->learn(data, cycles, initialLearning, finalLearning);
	cout << "Final error (MSE): " << error << endl;
	net->saveNet(outputNet);

}

void usageRecurrentBackpropagation(){
	cout << "Recurrent Backpropagation Options" << endl << endl;
	cout << "rNN B neuralNet neuralDescripion initialLearning finalLearning cycles outputNet datafile+" << endl << endl;
	cout << "\t neuralNet            the input neuralnet file. \"none\" to create a new one" << endl;
	cout << "\t neuralDescripion     the number of neurons of each layer separete by \":\". Example 10:20:3" << endl;
	cout << "\t initialLearning      initial learning rate. Range: [0,1]" << endl;
	cout << "\t finalLearning        final learning rate. Range: [0,1] and <= initialLearning" << endl;
	cout << "\t cycles               number of cycles. > 0" << endl;
	cout << "\t outputNet            filename for output net" << endl;
	cout << "\t datafile             datafiles (almost one)" << endl;
}

void recurrentBackpropagation(int argc, const char* argv[]){

	if(argc < 9){
		cerr << "Wrong arguments" << endl << endl;
		usageBackpropagation();
		return;
	}

	generateRandomSeed();

	//Network
	NeuralNet* net;
	net = new NeuralNet();

	string file(argv[2]);
	if(file != "none"){
		if(!net->loadNet(file)){
			cerr << "Net not loaded " << file  << endl;
			return;
		}
	}

	// net description
	int layers;
	int* neuronsPerLayer;
	int totalNeurons = loadNetDescription(&layers,&neuronsPerLayer, argv[3]);
	if(totalNeurons <= 0){
		cerr << "Neural description wrong" << endl;
		return;
	}
	if(layers < 3){
		cerr << "Wrong number of layers (>=3): "<< layers << endl;
		return;
	}

	if(file == "none"){
		net->createNet(totalNeurons);
		net->randomizeWeights(-1,1);
	}

	double initialLearning ;
	if(!getDouble(argv[4],&initialLearning) || initialLearning < 0 || initialLearning > 1){
		cerr << "Wrong initial learning" << endl;
		return;
	}

	double finalLearning;
	if(!getDouble(argv[5],&finalLearning) || finalLearning < 0 || finalLearning > initialLearning){
		cerr << "Wrong final learning" << endl;
		return;
	}

	int cycles ;
	if(!getInt(argv[6],&cycles) || cycles < 0){
		cerr << "Wrong number of cycles" << endl;
		return;
	}

	string outputNet(argv[7]);

	// data files
	int index = 8;
	Data* data = new Data();
	while(index < argc){
		string dataString(argv[index]);
		if(!data->loadData(dataString)){
			cerr << "Data file not loaded: " << dataString << endl;
			return;
		}else{
			cout << "Loaded: " << dataString << endl;
		}
		index++;
	}

	if(data->getInputSize() != neuronsPerLayer[0]){
		cerr << "Different input size in data and net: " << data->getInputSize() << " " << neuronsPerLayer[0] << endl;
		return;
	}

	if(data->getOutputSize() != neuronsPerLayer[layers-1]){
		cerr << "Different input size in data and net: " << data->getOutputSize() << " " << neuronsPerLayer[layers-1] << endl;
		return;
	}

	if(data->getInputSize() + data->getOutputSize() >= net->getNumberOfNeurons()){
		cerr << "Data input size + output size <= number of neurons of the net" << endl;
		return;
	}

	// neural training

	net->cleanState();
	LearningRecurrentBackpropagation* learn = new LearningRecurrentBackpropagation(net,neuronsPerLayer,layers);
	double error = learn->learn(data, cycles, initialLearning, finalLearning);
	cout << "Final error (MSE): " << error << endl;
	net->saveNet(outputNet);
}

void usageTestLearning(){
	cout << "Test Learning Options" << endl << endl;
	cout << "rNN tL neuralNet neuralDescripion initialLearning finalLearning cycles outputNet datafile+" << endl << endl;
	cout << "\t neuralNet            the input neuralnet file. \"none\" to create a new one" << endl;
	cout << "\t neuralDescripion     the number of neurons of each layer separete by \":\". Example 10:20:3" << endl;
	cout << "\t initialLearning      initial learning rate. Range: [0,1]" << endl;
	cout << "\t finalLearning        final learning rate. Range: [0,1] and <= initialLearning" << endl;
	cout << "\t forgetFactor         value to forget past patterns: [0,1], 1 remember all, 0 forget all" << endl;
	cout << "\t cycles               number of cycles. > 0" << endl;
	cout << "\t outputNet            filename for output net" << endl;
	cout << "\t datafile             datafiles (almost one)" << endl;
}

void testLearning(int argc, const char* argv[]){

	if(argc < 10){
		cerr << "Wrong arguments" << endl << endl;
		usageBackpropagation();
		return;
	}

	//generateRandomSeed(); // TODO random seed not generated in test mode

	//Network
	NeuralNet* net;
	net = new NeuralNet();

	string file(argv[2]);
	if(file != "none"){
		if(!net->loadNet(file)){
			cerr << "Net not loaded " << file  << endl;
			return;
		}
	}

	// net description
	int layers;
	int* neuronsPerLayer;
	int totalNeurons = loadNetDescription(&layers,&neuronsPerLayer, argv[3]);
	if(totalNeurons <= 0){
		cerr << "Neural description wrong" << endl;
		return;
	}
	if(layers < 3){
		cerr << "Wrong number of layers (>=3): "<< layers << endl;
		return;
	}

	if(file == "none"){
		net->createNet(totalNeurons);
		net->randomizeWeights(-1,1);
	}

	double initialLearning ;
	if(!getDouble(argv[4],&initialLearning) || initialLearning < 0 || initialLearning > 1){
		cerr << "Wrong initial learning" << endl;
		return;
	}

	double finalLearning;
	if(!getDouble(argv[5],&finalLearning) || finalLearning < 0 || finalLearning > initialLearning){
		cerr << "Wrong final learning" << endl;
		return;
	}

	int cycles ;
	if(!getInt(argv[6],&cycles) || cycles < 0){
		cerr << "Wrong number of cycles" << endl;
		return;
	}

	double forget ;
	if(!getDouble(argv[7],&forget) || cycles < 0){
		cerr << "Wrong number of cycles" << endl;
		return;
	}

	string outputNet(argv[8]);

	// data files
	int index = 9;
	int index2 = 0;
	Data** data = new Data*[argc-index];
	while(index < argc){
		string dataString(argv[index]);
		data[index2] = new Data();
		if(!data[index2]->loadData(dataString)){
			cerr << "Data file not loaded: " << dataString << endl;
			return;
		}else{
			cout << "Loaded: " << dataString << endl;
		}
		if(data[index2]->getInputSize() != neuronsPerLayer[0]){
			cerr << "Different input size in data and net: " << data[index2]->getInputSize() << " " << neuronsPerLayer[0] << endl;
			return;
		}
		if(data[index2]->getOutputSize() != neuronsPerLayer[layers-1]){
			cerr << "Different input size in data and net: " << data[index2]->getOutputSize() << " " << neuronsPerLayer[layers-1] << endl;
			return;
		}
		if(data[index2]->getInputSize() + data[index2]->getOutputSize() >= net->getNumberOfNeurons()){
			cerr << "Data input size + output size <= number of neurons of the net" << endl;
			return;
		}
		index++;
		index2++;
	}

	// neural training

	net->cleanState();
	LearningTest* learn = new LearningTest(net,neuronsPerLayer[0]);
	double error = learn->learn(data, index2, cycles, initialLearning, finalLearning, forget);
	cout << "Final error (MSE): " << error << endl;
	net->saveNet(outputNet);
}

void usageFusion(){
	cout << "fusion Options" << endl << endl;
	cout << "rNN fusion outpufile datafile+" << endl << endl;
	cout << "\t outpufile            output file name" << endl;
	cout << "\t datafile             datafiles (almost two)" << endl;
}

void fusion(int argc, const char* argv[]){

	if(argc < 5){
		cerr << "Wrong arguments" << endl << endl;
		usageFusion();
		return;
	}

	string file(argv[2]);

	int index = 3;
	Data* data = new Data();
	while(index < argc){
		string dataString(argv[index]);
		if(!data->loadData(dataString)){
			cerr << "Data file not loaded: " << dataString << endl;
			return;
		}else{
			cout << "Loaded: " << dataString << endl;
		}
		index++;
	}

	data->saveData(file);

	delete data;
}

void usageDataFilter(){
	cout << "dataFilter Options" << endl << endl;
	cout << "rNN dataFilter input inputSize (inputNumber)* output outputSize (outputNumber)* files prefix datafile+" << endl << endl;
	cout << "\t input                " << endl;
	cout << "\t inputSize            input size" << endl;
	cout << "\t inputNumber          input number to filter (add to filter file), none to add all" << endl;
	cout << "\t output               " << endl;
	cout << "\t outputSize           output size" << endl;
	cout << "\t outputNumber         output number to filter (add to filter file), none to add all" << endl;
	cout << "\t files                " << endl;
	cout << "\t prefix               prefix for generated files" << endl;
	cout << "\t datafile             datafiles (almost one)" << endl;
}

void dataFilter(int argc, const char* argv[]){

	if(argc < 8){
		cerr << "Wrong arguments" << endl << endl;
		usageDataFilter();
	}else{
		int inputSize;
		int outputSize;
		bool* selectedInputs;
		bool* selectedOutputs;
		int newInputSize = 0;
		int newOutputSize = 0;

		string inputString(argv[2]);
		if(inputString != "input"){
			cerr << "input word expected" << endl;
			return;
		}

		string auxString;
		int index = 3;

		if(!getInt(argv[index],&inputSize) || inputSize <= 0){
			cerr << "wrong input size (must be > 0): " << argv[index] << endl;
		}
		selectedInputs = new bool[inputSize];

		index++;
		if(index >= argc){
			cerr << "more arguments required" << endl;
			return;
		}
		auxString = argv[index];
		if(auxString == "output"){
			for(int i = 0; i < inputSize; i++){
				selectedInputs[i] = true;
			}
			newInputSize = inputSize;
			index++;
		}else{
			for(int i = 0; i < inputSize; i++){
				selectedInputs[i] = false;
			}
			auxString = argv[index];
			while(auxString != "output" && index < argc){
				int auxInt;
				if(!getInt(argv[index],&auxInt) || auxInt < 0 || auxInt >= inputSize){
					cerr << "wrong input data (must be >= 0 and  < " << inputSize << "): " << argv[index] << endl;
					return;
				}
				selectedInputs[auxInt] = true;
				index++;
				auxString = argv[index];
				newInputSize++;
			}
			index++;
		}

		if(index >= argc){
			cerr << "more arguments required" << endl;
			return;
		}

		if(!getInt(argv[index],&outputSize) || outputSize <= 0){
			cerr << "wrong output size (must be > 0): " << argv[index] << endl;
			return;
		}

		selectedOutputs = new bool[outputSize];

		index++;
		if(index >= argc){
			cerr << "more arguments required" << endl;
			return;
		}
		auxString = argv[index];
		if(auxString == "files"){
			for(int i = 0; i < outputSize; i++){
				selectedOutputs[i] = true;
			}
			index++;
			newOutputSize = outputSize;
		}else{
			for(int i = 0; i < outputSize; i++){
				selectedOutputs[i] = false;
			}
			auxString = argv[index];
			while(auxString != "files" && index < argc){
				int auxInt;
				if(!getInt(argv[index],&auxInt) || auxInt < 0 || auxInt >= outputSize){
					cerr << "wrong output data (must be >= 0 and  < " << outputSize << "): " << argv[index] << endl;
					return;
				}
				selectedOutputs[auxInt] = true;
				index++;
				auxString = argv[index];
				newOutputSize++;
			}
			index++;
		}

		if(index >= argc){
			cerr << "more arguments required" << endl;
			return;
		}

		string prefix(argv[index]);
		index++;
		if(index >= argc){
			cerr << "more arguments required" << endl;
			return;
		}

		// parse files
		for(int i = index; i < argc; i++){
			string file(argv[i]);
			Data data;
			if(!data.loadData(file)){
				cerr << "Data file " << file << " not loaded" << endl;
			}else if(data.getInputSize() != inputSize){
				cerr << "Different input size in file " << file << endl;
			}else if(data.getOutputSize() != outputSize){
				cerr << "Different output size in file " << file << endl;
			}else{
				double** auxDataInput = new double*[data.getNumberOfData()];
				double** auxDataOutput = new double*[data.getNumberOfData()];
				double** dataInput = data.getDataInput();
				double** dataOutput = data.getDataOutput();
				int m;
				for(int j = 0; j < data.getNumberOfData(); j++){
					auxDataInput[j] = new double[newInputSize];
					auxDataOutput[j] = new double[newOutputSize];
					m = 0;
					for(int k = 0; k < inputSize; k++){
						if(selectedInputs[k]){
							auxDataInput[j][m] = dataInput[j][k];
							m++;
						}
					}
					m = 0;
					for(int k = 0; k < outputSize; k++){
						if(selectedOutputs[k]){
							auxDataOutput[j][m] = dataOutput[j][k];
							m++;
						}
					}
				}
				Data* outputData = new Data(auxDataInput,auxDataOutput,data.getNumberOfData(),newInputSize,newOutputSize);
				string fileOutput("");
				fileOutput.append(prefix);
				fileOutput.append(file);
				if(!outputData->saveData(fileOutput)){
					cerr << "Data file " << fileOutput << " not saved properly" << endl;
				}else{
					cout << fileOutput << " generated" << endl;
				}
				delete outputData;
			}
		}
	}
}

void usageTest(){
	cout << "Test Options" << endl << endl;
	cout << "rNN Test neuralNet datafile+" << endl << endl;
	cout << "\t neuralNet            the input neuralnet file" << endl;
	cout << "\t datafile             datafiles (almost one)" << endl;
}

void test(int argc, const char* argv[]){

	if(argc < 4){
		cerr << "Wrong arguments" << endl << endl;
		usageTest();
	}else{
		bool ok = true;

		//load net
		string file(argv[2]);
		NeuralNet* net;
		net = new NeuralNet();
		if(file != "none"){
			if(!net->loadNet(file)){
				cerr << "Net not loaded " << file  << endl;
				ok = false;
			}
		}

		//load datafiles
		int index = 3;
		Data* data = new Data();
		while(ok && index < argc){
			string dataString(argv[index]);
			ok = data->loadData(dataString);
			if(!ok){
				cerr << "Data file not loaded: " << dataString << endl;
			}
			index++;
		}

		if(ok && data->getInputSize() + data->getOutputSize() >= net->getNumberOfNeurons()){
			ok = false;
			cerr << "Data input size + output size <= number of neurons of the net" << endl;
		}

		//test network
		if(ok){
			net->cleanState();
			double* netOutput = new double[data->getOutputSize()];
			double* output;
			double* input;
			double** inputData = data->getDataInput();
			double** outputData = data->getDataOutput();
			double error = 0;
			double* state = net->getState();
			int neurons = net->getNumberOfNeurons();
			bool show = data->getNumberOfData() <= 100;
			double* averageErrorOutput = new double[data->getOutputSize()];
			double* maxErrorOutput = new double[data->getOutputSize()];
			for(int i = 0; i < data->getOutputSize(); i++){
				averageErrorOutput[i] = 0;
				maxErrorOutput[i] = 0;
			}
			for(int i = 0; i < data->getNumberOfData(); i++){
				input = inputData[i];
				output = outputData[i];
				net->getOutput(input,data->getInputSize(),netOutput,data->getOutputSize());
				if(show){
					cout << "Inputs:  ";
					for(int j = 0; j < data->getInputSize(); j++){
						cout << "\t" << input[j];
					}
					cout << endl << "Network Outputs (pattern output, error): ";
				}
				double auxError = 0;
				double diff;
				double diff2;
				double diffabs;
				for(int j = 0; j < data->getOutputSize(); j++){
					diff = output[j] - netOutput[j];
					diff2 = diff*diff;
					averageErrorOutput[j] += diff2;
					diffabs = abs(diff);
					if(diffabs > maxErrorOutput[j]){
						maxErrorOutput[j] = diffabs;
					}
					auxError += diff2;
					if(show){
						cout << "\t" << netOutput[j] << " ( " << output[j] << " , " << diff << " ) ";
					}
				}
				if(show){
					cout << endl;
					cout << "state: ";
					for(int j = 0; j <= neurons; j++){
						cout << "\t" << state[j];
					}
					cout << endl;
				}
				error += auxError / data->getOutputSize();
			}
			for(int i = 0; i < data->getOutputSize(); i++){
				averageErrorOutput[i] /= data->getNumberOfData();
			}
			error /= data->getNumberOfData();
			cout << endl << "-----------------------------------------------" << endl << endl;
			cout << "Number of patterns: " << data->getNumberOfData() << endl;
			cout << "Error (MSE):        " << error << endl;
			cout << "Error per output (average and maximum): " << endl;
			for(int i = 0; i < data->getOutputSize(); i++){
				cout << "\t" << i << ": \t" << averageErrorOutput[i] << " \t( " << maxErrorOutput[i] << " )" << endl;
			}
			cout << endl;
			delete netOutput;
			delete averageErrorOutput;
			delete maxErrorOutput;
		}
		delete net;
	}
}


void usageAnalyze(){
	cout << "Analize Options" << endl << endl;
	cout << "rNN analyze datafile" << endl << endl;
	cout << "\t datafile             datafile" << endl;
}

void analyze(int argc, const char* argv[]){

	if(argc != 3){
		cerr << "Wrong arguments" << endl << endl;
		usageAnalyze();
	}else{
		bool ok = true;

		//load datafile
		Data* data = new Data();
		string file(argv[2]);
		ok = data->loadData(file);
		if(!ok){
			cerr << "Data file not loaded: " << file << endl;
			return;
		}

		int inputSize = data->getInputSize();
		int outputSize = data->getOutputSize();
		int patterns = data->getNumberOfData();
		double** inputs = data->getDataInput();
		double** outputs = data->getDataOutput();

		double distanceIn, distanceOut, distanceBoth;
		double* distancesInputs = new double[patterns];
		double* distancesOutputs = new double[patterns];
		double* distancesBoth = new double[patterns];

		for(int i = 0; i < patterns; i++){
			distancesInputs[i] = 0;
			distancesOutputs[i] = 0;
			distancesBoth[i] = 0;
			for(int j = 0; j < patterns; j++){
				if(i!=j){
					distanceIn = 0;
					distanceOut = 0;
					distanceBoth = 0;
					for(int k = 0; k < inputSize; k++){
						distanceIn += (inputs[i][k] - inputs[j][k]) * (inputs[i][k] - inputs[j][k]);
					}
					for(int k = 0; k < outputSize; k++){
						distanceOut += (outputs[i][k] - outputs[j][k]) * (outputs[i][k] - outputs[j][k]);
					}
					distanceBoth = distanceIn+distanceOut;

					distancesInputs[i] += sqrt(distanceIn);
					distancesOutputs[i] += sqrt(distanceOut);
					distancesBoth[i] += sqrt(distanceBoth);
				}
			}
			distancesInputs[i] /= (patterns-1);
			distancesOutputs[i] /= (patterns-1);
			distancesBoth[i] /= (patterns-1);
		}

		double averageIn = 0, averageOut = 0, averageBoth = 0;
		double minIn = distancesInputs[0], minOut = distancesOutputs[0], minBoth = distancesBoth[0];
		double maxIn = distancesInputs[0], maxOut = distancesOutputs[0], maxBoth = distancesBoth[0];
		double stdevIn = 0, stdevOut = 0, stdevBoth = 0;

		for(int i = 0; i < patterns; i++){
			averageIn += distancesInputs[i];
			averageOut += distancesOutputs[i];
			averageBoth += distancesBoth[i];
			if(minIn > distancesInputs[i]){
				minIn = distancesInputs[i];
			}else if(maxIn < distancesInputs[i]){
				maxIn = distancesInputs[i];
			}
			if(minOut > distancesOutputs[i]){
				minOut = distancesOutputs[i];
			}else if(maxOut < distancesOutputs[i]){
				maxOut = distancesOutputs[i];
			}
			if(minBoth > distancesBoth[i]){
				minBoth = distancesBoth[i];
			}else if(maxBoth < distancesBoth[i]){
				maxBoth = distancesBoth[i];
			}
		}
		averageIn /= patterns;
		averageOut /= patterns;
		averageBoth /= patterns;
		for(int i = 0; i < patterns; i++){
			stdevIn += (distancesInputs[i] - averageIn) * (distancesInputs[i] - averageIn);
			stdevOut += (distancesOutputs[i] - averageOut) * (distancesOutputs[i] - averageOut);
			stdevBoth += (distancesBoth[i] - averageBoth) * (distancesBoth[i] - averageBoth);
		}

		stdevIn = sqrt(stdevIn/(patterns-1));
		stdevOut = sqrt(stdevOut/(patterns-1));
		stdevBoth = sqrt(stdevBoth/(patterns-1));

		cout << "Inputs:     Mean: " << averageIn << " \t Min: " << minIn << " \t Max: " << maxIn << " \t Stdev: " << stdevIn << endl;
		cout << "Outputs:    Mean: " << averageOut << " \t Min: " << minOut << " \t Max: " << maxOut << " \t Stdev: " << stdevOut << endl;
		cout << "Both:       Mean: " << averageBoth << " \t Min: " << minBoth << " \t Max: " << maxBoth << " \t Stdev: " << stdevBoth << endl;

		delete distancesInputs;
		delete distancesOutputs;
		delete distancesBoth;


	}
}

void weka(int argc, const char* argv[]){

	if(argc != 3){
		cerr << "Wrong arguments" << endl << endl;
		cout << "weka Options" << endl << endl;
		cout << "rNN weka datafile" << endl << endl;
		cout << "\t datafile             datafile" << endl;
		return;
	}

	string file(argv[2]);
	Data* data = new Data();
	if(!data->loadData(file)){
		cerr << "Data file not loaded: " << file << endl;
		return;
	}else{
		cout << "Loaded: " << file << endl;
	}


	int inputSize = data->getInputSize();
	int outputSize = data->getOutputSize();
	int patterns = data->getNumberOfData();
	if(inputSize != 28 || outputSize != 3){
		cerr << "Wrong number of inputs or outpus" << endl << endl;
		return;
	}

	double** dataInput = data->getDataInput();
	double** dataOutput = data->getDataOutput();

	std::ofstream output[3];
	output[0].open("weka.0.arff");
	output[1].open("weka.1.arff");
	output[2].open("weka.2.arff");

	bool ok = true;
	for(int i = 0; i < outputSize; i++){
		ok = ok && 	output[i].good();
	}

	if(ok){
		output[0] << "@RELATION accelerate_break" << endl;
		output[1] << "@RELATION gear" << endl;
		output[2] << "@RELATION steering" << endl;
		for(int i = 0; i < outputSize; i++){
			output[i] << "@ATTRIBUTE speed NUMERIC" << endl;
			output[i] << "@ATTRIBUTE angle_axis NUMERIC" << endl;
			output[i] << "@ATTRIBUTE gear NUMERIC" << endl;
			output[i] << "@ATTRIBUTE lateral_speed NUMERIC" << endl;
			output[i] << "@ATTRIBUTE rpm NUMERIC" << endl;
			for(int j = 0; j < 4; j++){
				output[i] << "@ATTRIBUTE spin_" << j << " NUMERIC" << endl;
			}
			for(int j = 0; j < 19; j++){
				output[i] << "@ATTRIBUTE sensor_" << j << " NUMERIC" << endl;
			}
		}
		output[0] << "@ATTRIBUTE class_accelerate_break NUMERIC" << endl;
		output[1] << "@ATTRIBUTE class_gear NUMERIC" << endl;
		output[2] << "@ATTRIBUTE class_steering NUMERIC" << endl;
		for(int k = 0; k < outputSize; k++){
			output[k] << "@DATA" << endl;
		}
		for(int i = 0; i < patterns; i++){
			for(int k = 0; k < outputSize; k++){
				for(int j = 0; j < inputSize; j++){
					output[k] << dataInput[i][j] << ", ";
				}
			}
			for(int k = 0; k < outputSize; k++){
				output[k] << dataOutput[i][k] << endl;
			}
		}
	}else{
		std::cerr << "Output files not opened: "<< std::endl;
	}
	for(int i = 0; i < outputSize; i++){
		output[i].close();
	}

	delete data;
}

void data(int argc, const char* argv[]){

	if(argc != 3){
		cerr << "Wrong arguments" << endl << endl;
		cout << "data Options" << endl << endl;
		cout << "rNN data datafile" << endl << endl;
		cout << "\t datafile             datafile" << endl;
		return;
	}

	string file(argv[2]);
	Data* data = new Data();
	if(!data->loadData(file)){
		cerr << "Data file not loaded: " << file << endl;
		return;
	}else{
		cout << "Loaded: " << file << endl;
	}

	/*

	int inputSize = data->getInputSize();
	int outputSize = data->getOutputSize();
	int patterns = data->getNumberOfData();

	double** dataInput = data->getDataInput();
	double** dataOutput = data->getDataOutput();

	double distanceLimit = 0.1;

	*/
	// TODO
	vector<double*> inputsVector;
	vector<double*> outputsVector;





	delete data;
}



/**
 * Main method of the program. First argument must be the algorithm.
 */
int main( int argc, const char* argv[] )
{
	if(argc < 2){
		usage();
		return 0;
	}
	string algorithm(argv[1]);

	if(algorithm == "B"){
		backpropagation(argc,argv);
	}else if(algorithm == "rB"){
		recurrentBackpropagation(argc,argv);
	}else if(algorithm == "lT"){
		testLearning(argc,argv);
	}else if(algorithm == "dataFilter"){
		dataFilter(argc,argv);
	}else if(algorithm == "fusion"){
		fusion(argc,argv);
	}else if(algorithm == "test"){
		test(argc,argv);
	}else if(algorithm == "analyze"){
		analyze(argc,argv);
	}else if(algorithm == "weka"){
		weka(argc,argv);
	}else if(algorithm == "data"){
		data(argc,argv);
	}else{
		cerr << "Wrong algorithm" << endl << endl;
		usage();
		return -1;
	}
	return 0;
}
