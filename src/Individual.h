#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

/**
 * A class which represents an indivual of the population
 */
class Individual
{
protected:

	/**
	 * Default constructor, it creates a new random chromosome.
	 */
	Individual(int objectives = 0);
	
public:
	
	/**
	 * Default destructor
	 */
	virtual ~Individual();
	
	/**
	 * Returns true if the fitness is setted.
	 */
	inline bool isSetFitness()
	{
		return	fitnessSetted;
	}
		
	/**
	 * Sets the fitness.
	 */
	inline void setFitness(double fitness)
	{
		this->fitness = fitness;
		fitnessSetted = true;
	}
	
	/**
	 * Unsets the fitness.
	 */
	inline void unsetFitness()
	{
		fitness = -1;
		fitnessSetted = false;
	}
	
	/**
	 * Returns the fitness.
	 */
	inline double getFitness()
	{
		return fitness;	
	}
	
	/**
	 * Copy this individual data into another
	 */
	virtual void copy(Individual* individual);
		
	/**
	 * Sets the dominance.
	 */
	inline void setDominance(int dominance){
		this->dominance = dominance;	
	}
	
	/**
	 * Returns the dominance.
	 */
	inline int getDominance(){
		return this->dominance;	
	}
		
	/**
	 * Sets the distance.
	 */
	inline void setDistance(double distance){
		this->distance = distance;	
	}
	
	/**
	 * Returns the distance.
	 */
	inline double getDistance(){
		return this->distance;	
	}
	
	/**
	 * Sets an objective if number is greater or equal than 0 and less than numberOfObjectives.
	 */
	void setObjective(int number, double objective);
	
	/**
	 * Returns a objetive or 0 if it does not exits.
	 */
	double getObjective(int number);
	
	/**
	 * Returns the number of objectives.
	 */
	inline int getNumberOfObjectives(){
		return this->numberOfObjectives;
	}
	
	
private:

	/**
	 * Variable to know if the fitness has been setted.
	 */
	bool fitnessSetted;
	
	/**
	 * Fitness value for this invididual.
	 */
	double fitness;
	
	/**
	 * Dominance value for this individual.
	 */
	int dominance;
	
	/**
	 * Distance value for this individual. Normalized between 0 and 1.
	 */
	double distance;
	
	/**
	 * Array with the objetive values.
	 */
	double* objectives;
	
	/**
	 * Number of objectives.
	 */
	int numberOfObjectives;
	
};

#endif /*INDIVIDUAL_H_*/
