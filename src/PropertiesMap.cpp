#include "PropertiesMap.h"
#include <fstream>


PropertiesMap::PropertiesMap()
{
	
}

PropertiesMap::~PropertiesMap()
{
	
}

PropertiesMap* PropertiesMap::singleton = NULL;

PropertiesMap* PropertiesMap::instance()
{
	if( singleton == NULL){
		singleton = new PropertiesMap();
	}	
	return singleton;
}

bool PropertiesMap::save(string file)
{
	bool ok = false;
	ofstream output(file.c_str());
	ok = output.good();
	if(ok){
		map<string,string>::iterator iter;   
  		for( iter = propertiesMap.begin(); iter != propertiesMap.end(); iter++ ) {
    		output << iter->first << " = " << iter->second << endl;
  		}		
		output.close();
	}	
	return ok;
}

bool PropertiesMap::load(string file)
{
	bool ok = false;
	ifstream input;
	string s;
	string key;
	string *value;
	input.open(file.c_str());
	ok = input.good();
	if(ok){
		while( !input.eof() ){
			getline(input,s);
			while( (s.length() == 0 || s[0] == '#') && !input.eof() ){
				getline(input,s);
			}
			if(!input.eof()){
				value = new string;
				int pos = s.find("=",0);
				key = s.substr(0,pos);
				*value = s.substr(pos+1);
				trimString(key);
				trimString(*value);
				setProperty(key,*value);
			}
		}
	}
	input.close();
	return ok;
}

void PropertiesMap::setProperty(string key, string value)
{
	if(key.length() > 0){
		delProperty(key);
		propertiesMap[key] = value;
	}
}

void PropertiesMap::delProperty(string key)
{
//	if( propertiesMap.count(key) > 0){
//		delete &propertiesMap[key];
//	}
	propertiesMap.erase(key);
}

const string PropertiesMap::get(string key)
{
	string value = "";
	if( propertiesMap.count(key) > 0){
		value = propertiesMap[key];
	}
	return value;
}

void PropertiesMap::trimString(string& str)
{
	string::size_type pos = str.find_last_not_of(' ');
	if(pos != string::npos) {
	    str.erase(pos + 1);
	    pos = str.find_first_not_of(' ');
	    if(pos != string::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());
}
