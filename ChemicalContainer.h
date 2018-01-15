#ifndef __H__ChemicalContainer
#define __H__ChemicalContainer

#include "include.h"

#define contains_amount 2

#define FOOD_CHEMCON_ID 0
#define CO2_CHEMCON_ID 1

#define TemperatureDiffusionCoefficient 1

const static string writtenSubstances[] = { "Food", "CO2" };

const static float ATPCostToMoveSubstances[] = { 1.0f, 1.0f };

class World;
class Membrane;

class ChemicalContainer
{
	float volume; //this is given in um
	float temperature; //this is given in degree Celcius, so that we operate closer to the 0 and thus the float is more precises
	float surfaceArea;

	int amountOfNeightbouringChunks = 0;

	World* world;

	double contains[contains_amount]; //different concentrations of fluids are stored here
	double containsBuffer[contains_amount];

	static const float diffusionCoefficients[contains_amount];

	ChemicalContainer* surroundingChunks[24]; //this only applies if the container in question actually is a chunk, if its a cell this remains empty
	//the vector is ordered like this: 0-5 are the 6 direct neighbours

public:
	ChemicalContainer(World* world, float volume, float surfaceArea, ChemicalContainer* splittingFrom = nullptr);
	
	void DiffuseToNeighbouringChunks(float t);

	void DiffuseFromAndTo(ChemicalContainer* target, float t);
	void DiffuseFromAndTo(ChemicalContainer* target, float t, float surfaceArea);
	float DiffuseFromAndTo(ChemicalContainer* target, float t, Membrane* membrane);

	float GetSwellAmount(ChemicalContainer* target);

	void ApplyContains() 
	{ 
		for (int i = 0; i < contains_amount; i++)
		{
			contains[i] = containsBuffer[i];
		}
	};

	void AddSurroundingChunk(ChemicalContainer* newCem) 
	{ 
		surroundingChunks[amountOfNeightbouringChunks] = newCem;
		amountOfNeightbouringChunks++; 
	}

	double* GetContains() { return contains; }
	double GetContains(int pos) { return contains[pos]; }

	float GetTemperature() { return temperature; }
	void AddToTemperature(float add) { temperature += add; }

	float GetVolume() { return volume; }
	void AddSubstanceToContains(int key, float add) { contains[key] += add; containsBuffer[key] = contains[key]; }
	void SetSubstanceInContains(int key, float set) { contains[key] = set; containsBuffer[key] = contains[key]; }

	~ChemicalContainer();
};

#endif