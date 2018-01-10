#ifndef __H__Membrane
#define __H__Membrane

#include "include.h"
#include "Trait.h"
#include "ChemicalContainer.h"
#include "NeuralNetwork.h"

class DNA;
class Cell;
class World;

class Membrane : public Trait //the full name for this is semipermeable membrane
{
	Cell* pCell = nullptr;
	World* world = nullptr;
	NeuralNetwork* neuralNet = nullptr;

	float surfaceArea = 0;
	float totalATPCost = 0;

	int outputNodeArr[contains_amount];
	float modifierArr[contains_amount];

public:
	Membrane(Cell* parentCell, DNA* dna, int startingPos);

	void InputValuesToNeuralNetwork() {}

	int GetOutputNode(int ind) { return neuralNet->GetOutputNode(outputNodeArr[ind]); }
	float GetModifier(int ind) { return modifierArr[ind]; }

	float Tick(int t);
	string GetOutputString();
	float GetATPBuildingCost() { return 0.0f; }

	~Membrane();
};

#endif