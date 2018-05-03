#ifndef __H__SplittingManager
#define __H__SplittingManager

#include "IncludeTraits.h"
#include "include.h"

class Cell;
class NeuralNetwork;
class DNA;

#define Building_Coeffiecent 0.1f

class SplittingManager : public Trait, public NeuralNetworkInput
{
	Cell* pCell = nullptr;
	NeuralNetwork* neuralNet = nullptr;
	DNA* dna = nullptr;

	bool createdThroughDNA = false;
	bool forceSplit = false;

	float totalCost = 0.0f;
	float surface = 0.0f;

	int randomSpawnChance = 300000;
	float buildingSpeed = 0.4f;

	Cell* splittingCell = nullptr;
	int buildingOutputNode = 0;
	int splittingOutputNode = 0;
	int buildingInputNode = 0;

	int lastCheckedForSpace = 0;

	void SplitCellOff(int t);

public:
	SplittingManager(Cell* parentCell, DNA* dna, int startpos);
	SplittingManager(Cell* parentCell);

	int GetType() { return Type_SplittingManager; }

	float Tick(int t);
	string GetOutputString();

	void InputValuesToNeuralNetwork();

	float GetATPBuildingCost() { return (pCell->GetVolume() / 100.0f) + static_cast<int>(createdThroughDNA) * (pCell->GetVolume() / 100.0f); }
	float GetRandomSpawnChance() { return randomSpawnChance; }

	void StartSplitting() { forceSplit = true; }
	bool IsSplitting() { return splittingCell == nullptr; }

	bool GetDNAInduced() { return createdThroughDNA; }

	~SplittingManager();
};

#endif