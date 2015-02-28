#include <iostream>
#include <sstream>
#include <stdlib.h>
using namespace std;

#include "PropertiesMap.h"
#include "mtrand.h"
#include "AlgorithmTSPAIS.h"
#include "AlgorithmTSPAIS2.h"
#include "AlgorithmTSPGA.h"
#include "AlgorithmTSPGA2.h"

/**
 * Prints default usage of the program.
 */
void usage(){
	cout << "Usage: AIS-TravelSalesman algorithm propertiesfile" << endl << endl;
	cout << "\t algorithm: " << endl;
	cout << "\t            AISi    artificial immune algorithm" << endl;
	cout << "\t            AISd    artificial immune algorithm with random keys" << endl;
	cout << "\t            GAi     genetic algorithm" << endl;
	cout << "\t            GAd     genetic algorithm with random keys" << endl;
	cout << endl;
}

/**
 * Main method of the program. First argument must be the algorithm and the second must be the properties file.
 */
int main( int argc, const char* argv[] )
{
	string algorithm;
	string properties;
	if(argc != 3){
		usage();
	}else{
		algorithm = argv[1];
		properties = argv[2];

		cout << "Loading properties file: "<< properties << endl;

		bool ok = PropertiesMap::instance()->load(properties);
		if(!ok){
			cerr << "Error loading properties file" << endl;
		}else{
			cout << "Properties file loaded" << endl;
		}

		string aux;
		unsigned long int seed = 0;
		aux = PropertiesMap::instance()->get("randomSeed");
		if(aux.length() > 0){
			char* finalPtr = 0;
			seed = strtol(aux.c_str(),&finalPtr, 10);
		}
		if(seed == 0){
			cout << "Generating random seed..." << endl;
			seed = time(NULL);

			std::stringstream buf;
			buf << seed;
			string seedstring = buf.str();

			cout << seedstring << endl;
			PropertiesMap::instance()->setProperty("randomSeed",seedstring);
		}
		MTRand_int32 irand(seed);

		cout << "Saving properties file..." << endl;
		PropertiesMap::instance()->save(properties+".last");
		cout << "Saved properties file as " << properties << ".last" << endl;

		cout << "Algorithm: " << algorithm << endl;

		if(algorithm == "AISi"){
				AlgorithmTSPAIS* a = new AlgorithmTSPAIS();
				a->run();
		}else if (algorithm == "AISd"){
				AlgorithmTSPAIS2* a = new AlgorithmTSPAIS2();
				a->run();
		}else if (algorithm == "GAi"){
				AlgorithmTSPGA* a = new AlgorithmTSPGA();
				a->run();
		}else if (algorithm == "GAd"){
				AlgorithmTSPGA2* a = new AlgorithmTSPGA2();
				a->run();
		}else{
			cerr << "No valid algorithm selected" << endl;
		}

	}
	return 0;
}

