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

	//active Movement
	int outputNodeArr[contains_amount];
	float modifierArrCellToChunk[contains_amount];
	float modifierArrChunkToCell[contains_amount];

	//passiv Movement
	float modifierArrPassiveMovement[contains_amount];
public:
	Membrane(Cell* parentCell, DNA* dna, int startingPos);

	void InputValuesToNeuralNetwork() {}

	float GetOutputNode(int ind) { return neuralNet->GetOutputNode(outputNodeArr[ind]); }
	float GetModifierCellToChunk(int ind) { return modifierArrCellToChunk[ind]; }
	float GetModifierChunkToCell(int ind) { return modifierArrChunkToCell[ind]; }
	float GetModifierPassiv(int ind) { return modifierArrPassiveMovement[ind]; }
	float GetSurfaceArea() { return surfaceArea; }

	float Tick(int t);
	string GetOutputString();
	float GetATPBuildingCost() { return 0.0f; }

	~Membrane();
};

#endif