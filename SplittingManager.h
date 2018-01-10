#ifndef __H__SplittingManager
#define __H__SplittingManager

#include "IncludeTraits.h"
#include "include.h"

class Cell;
class NeuralNetwork;
class DNA;

class SplittingManager : public Trait, public NeuralNetworkInput
{
	Cell* pCell = nullptr;
	NeuralNetwork* neuralNet = nullptr;
	DNA* dna = nullptr;

	float totalCost = 0.0f;

	Cell* splittingCell = nullptr;
	int buildingOutputNode = 0;
	int buildingInputNode = 0;

public:
	SplittingManager(Cell* parentCell, DNA* dna, int startpos);

	float Tick(int t);
	string GetOutputString();

	void InputValuesToNeuralNetwork() {}

	float GetATPBuildingCost() { return 0.0f; }


	~SplittingManager();
};

#endif