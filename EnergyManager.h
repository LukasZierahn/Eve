#ifndef __H__EnergyManager
#define __H__EnergyManager

#include "Trait.h"
#include "Cell.h"
#include "ChemicalContainer.h"

#define CONVERSION_CONSTANT 2.0f
#define CONVERSION_UPKEEP 0.001f

class DNA;
class NeuralNetwork;

class EnergyManager : public Trait
{
	Cell* pCell = nullptr;
	NeuralNetwork* neuralNet = nullptr;
	ChemicalContainer* chemCon = nullptr;


	bool createdThroughDNA = false;

	int conversionNeuralNode = 0;
	float conversionCapabilities = 0;

public:
	EnergyManager(Cell* parentCell, DNA* dna, int startpos);
	EnergyManager(Cell* parentCell);

	float Tick(int t);
	string GetOutputString();
	float GetATPBuildingCost() { return pCell->GetChemCon()->GetVolume() / 10 + (pCell->GetChemCon()->GetVolume() / 50) * conversionCapabilities; }

	int GetType() { return Type_EnergyManager; }

	~EnergyManager();
};

#endif