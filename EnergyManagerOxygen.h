#ifndef __H__EnergyManagerOxygen
#define __H__EnergyManagerOxygen

#include "Trait.h"
#include "Cell.h"
#include "ChemicalContainer.h"

class DNA;
class NeuralNetwork;

#define Conversion_Constant_Oxygen 4.0f 
#define Conversion_Upkeep_Oxygen 0.0001f 

class EnergyManagerOxygen : public Trait
{
	Cell* pCell = nullptr;
	NeuralNetwork* neuralNet = nullptr;
	ChemicalContainer* chemCon = nullptr;

	int conversionNeuralNode = 0;
	float conversionCapabilities = 0;

public:
	EnergyManagerOxygen(Cell* parentCell, DNA* dna, int startpos);

	float Tick(int t);
	string GetOutputString();
	float GetATPBuildingCost() { return pCell->GetChemCon()->GetVolume() + pCell->GetChemCon()->GetVolume() * conversionCapabilities; }

	float GetConversionCapabilities() { return conversionCapabilities; }

	int GetType() { return Type_EnergyManagerOxygen; }

	~EnergyManagerOxygen();
};

#endif