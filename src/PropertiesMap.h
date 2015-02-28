#ifndef PROPERTIESMAP_H_
#define PROPERTIESMAP_H_

#include <map>
#include <string>
using namespace std;

/**
 * A map with properties. Line length must be less than 1024 bytes
 */
class PropertiesMap
{
	
protected:

	/**
	 * Default constructor
	 */
	PropertiesMap();
	
	/**
	 * Default destructor
	 */
	virtual ~PropertiesMap();
	
private:
	
	/**
	 * Singleton variable;
	 */
	static PropertiesMap *singleton;
	
	
public:

	/**
	 * Returns the unique PropertiesMap instance.
	 */
	static PropertiesMap* instance();
	
	/**
	 * Save the properties into a file.
	 */
	bool save(const string file);
	
	/**
	 * Load properties from a file. Line length must be less than 1024 bytes
	 */
	bool load(const string file);
	
	/**
	 * Append one property or modified it if it was defined previously.
	 */
	void setProperty(const string key, const string value);
	
	/**
	 * Delete a property
	 */
	void delProperty(const string key);
	
	/**
	 * Return a property with the defined key or a null string if it does not exists.
	 */
	const string get(const string key);


private:

	/**
	 * Variable where store the properties.
	 */
	map<string, string> propertiesMap;	
	
	/**
	 * Trims a string.
	 */
	void trimString(string& s);
	
};


#endif /*PROPERTIESMAP_H_*/
