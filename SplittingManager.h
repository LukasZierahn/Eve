#ifndef __H__SplittingManager
#define __H__SplittingManager

#include "IncludeTraits.h"
#include "include.h"

class Cell;
class NeuralNetwork;
class DNA;

#define Building_Coeffiecent 0.05f

class SplittingManager : public Trait, public NeuralNetworkInput
{
	Cell* pCell = nullptr;
	NeuralNetwork* neuralNet = nullptr;
	DNA* dna = nullptr;

	bool createdThroughDNA = false;
	bool forceSplit = false;

	float totalCost = 0.0f;
	float surface = 0.0f;

	int randomSpawnChance = 120000;

	Cell* splittingCell = nullptr;
	int buildingOutputNode = 0;
	int splittingOutputNode = 0;
	int buildingInputNode = 0;

public:
	SplittingManager(Cell* parentCell, DNA* dna, int startpos);
	SplittingManager(Cell* parentCell);

	int GetType() { return Type_SplittingManager; }

	float Tick(int t);
	string GetOutputString();

	void InputValuesToNeuralNetwork() {}

	float GetATPBuildingCost() { return 100.0f; }

	void StartSplitting() { forceSplit = true; }

	~SplittingManager();
};

#endif