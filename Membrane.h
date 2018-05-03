#ifndef __H__Membrane
#define __H__Membrane

#include "include.h"
#include "Trait.h"
#include "ChemicalContainer.h"
#include "NeuralNetwork.h"
#include "Cell.h"

#define Swell_Speed_Factor 0.0000001f
#define Swell_Cost_Factor 0.01f

#define Membrane_ATP_Cost_Modfier 0.000005f

class DNA;
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

	float swellingModifier;
	int swellingOutputNode;

	//passiv Movement
	float modifierArrPassiveMovement[contains_amount];
public:
	Membrane(Cell* parentCell, DNA* dna, int startingPos);

	void InputValuesToNeuralNetwork() {}

	Cell* GetParentCell() { return pCell; }

	float GetOutputNode(int ind) { return neuralNet->GetOutputNode(outputNodeArr[ind]); }
	float GetModifierCellToChunk(int ind) { return modifierArrCellToChunk[ind]; }
	float GetModifierChunkToCell(int ind) { return modifierArrChunkToCell[ind]; }
	float GetModifierPassiv(int ind) { return modifierArrPassiveMovement[ind]; }
	float GetSurfaceArea() { return surfaceArea; }

	float Tick(int t);
	string GetOutputString();
	float GetATPBuildingCost() { return surfaceArea / 10; }

	int GetType() { return Type_Membrane; }

	~Membrane();
};

#endif