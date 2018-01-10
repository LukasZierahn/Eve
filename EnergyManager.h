#ifndef __H__EnergyManager
#define __H__EnergyManager

#include "Trait.h"
#include "Cell.h"
#include "ChemicalContainer.h"

#define CONTAINS_TO_ABSOLUTE_NUMBERS 1000

#define CONVERSION_CONSTANT 10.0f

class DNA;
class NeuralNetwork;

class EnergyManager : public Trait
{
	Cell* pCell = nullptr;
	NeuralNetwork* neuralNet = nullptr;
	ChemicalContainer* chemCon = nullptr;

	int conversionNeuralNode = 0;

	float conversionCapabilities = 0;

public:
	EnergyManager(Cell* parentCell, DNA* dna, int startpos);

	float Tick(int t);
	string GetOutputString();
	float GetATPBuildingCost() { return pCell->GetChemCon()->GetVolume() / 100.0f; }

	~EnergyManager();
};

#endif