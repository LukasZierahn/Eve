#ifndef  __H__ChemicalContainer
#define __H__ChemicalContainer

#include "include.h"

#define number_of_substances 3

#define sodium_cations 0
#define chlorid_anions 1
#define sulfate 2

static string writtenSubstances[]  = {"Na", "Cl", "Sulfate"};

//all of the following are given in mass concentration and represent todays average sea water, source: https://en.wikipedia.org/wiki/Seawater
#define sodium_cations_concentration 1.925
#define chlorid_anions_concentration 1.071
#define sulfate_concentration 0.2695

//these are given in g/mol
#define sodium_cations_atomic_weigth 22.990
#define chlorid_anions_atomic_weigth 35.45
#define sulfate_atomic_weigth 32.06 + 4 * 15.999 //1 sulfur + 4 oxygens

class World;

class ChemicalContainer
{
	int volume; //this is given in um
	int pressure;

	short amountOfNeightbouringChunks = 0;

	World* world;

	double contains[number_of_substances]; //different concentrations of fluids are stored here
	double containsBuffer[number_of_substances];

	//these are given in um/s^2
	static const float diffusionCoefficients[number_of_substances];

	//these are given in ms
	float diffusionTimes[number_of_substances];

	ChemicalContainer* surroundingChunks[24]; //this only applies if the container in question actually is a chunk, if its a cell this remains empty
	//the vector is ordered like this: 0-5 are the 6 direct neighbours, 6-13 are the 8 neighbours that are root(2) away and 14-23 are the 8 corners

public:
	ChemicalContainer(World* world, int volume);
	
	void DiffuseToNeighbouringChunks(float t);

	void DiffuseFromAndTo(ChemicalContainer* target, float t);

	void ApplyContains() 
	{ 
		for (int i = 0; i < number_of_substances; i++)
		{
			contains[i] = containsBuffer[i];
		}
	};

	void AddSurroundingChunk(ChemicalContainer* newCem) { surroundingChunks[amountOfNeightbouringChunks] = newCem; amountOfNeightbouringChunks++; };

	double* GetContains() { return contains; };
	void AddSubstanceToContains(int key, float add) { contains[key] += add; containsBuffer[key] = contains[key]; };
	void SetSubstanceInContains(int key, float set) { contains[key] = set; containsBuffer[key] = contains[key]; };

	~ChemicalContainer();
};

#endif