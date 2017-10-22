#ifndef  __H__ChemicalContainer
#define __H__ChemicalContainer

#include "include.h"

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
	int volume;
	int pressure;
	float temperature;

	World* world;

	map<string, float> contains; //different concentrations of fluids are stored here

	vector<ChemicalContainer*> surroundingChunks; //this only applies if the container in question actually is a chunk, if its a cell this remains empty
	//the vector is ordered like this: 0-5 are the 6 direct neighbours, 6-13 are the 8 neighbours that are root(2) away and 14-23 are the 8 corners

public:
	ChemicalContainer(World* world, int volume);
	
	void DispatchPushMessagesToChunks(float t);
	void AddSurroundingChunk(ChemicalContainer* newCem) { surroundingChunks.push_back(newCem); };

	map<string, float>* GetContains() { return &contains; };
	void AddSubstanceToContains(string key, float add) { contains.at(key) += add; };
	void SetSubstanceInContains(string key, float set) { contains.at(key) = set; };

	~ChemicalContainer();
};

struct ChemicalPush
{
	ChemicalContainer* source;
	ChemicalContainer* target;
	map<string, float> contains;
};

#endif