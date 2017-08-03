#ifndef  __H__ChemicalContainer
#define __H__ChemicalContainer

#include "include.h"

class ChemicalContainer
{
	int volume;
	int pressure;
	float temperature;

	map<string, float> contains; //different concentrations of fluids are stored here

	vector<ChemicalContainer*> surroundingChunks;

public:
	ChemicalContainer(int volume);
	void AddsurroundingChunk(ChemicalContainer* newCem) { surroundingChunks.push_back(newCem); };
	~ChemicalContainer();
};

#endif