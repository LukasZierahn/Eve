#ifndef  __H__ChemicalContainer
#define __H__ChemicalContainer

#include "include.h"

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
	
	void DispatchPushMessagesToChunks(int t);
	void AddsurroundingChunk(ChemicalContainer* newCem) { surroundingChunks.push_back(newCem); };

	map<string, float>* GetContains() { return &contains; };

	~ChemicalContainer();
};

struct ChemicalPush
{
	ChemicalContainer* source;
	ChemicalContainer* target;
	map<string, float> contains;
};

#endif