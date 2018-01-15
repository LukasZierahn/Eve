#include "Membrane.h"
#include "Cell.h"
#include "NeuralNetwork.h"
#include "ChemicalContainer.h"
#include "Chunk.h"
#include "World.h"
#include "DNA.h"

Membrane::Membrane(Cell* parentCell, DNA* dna, int startingPos) : pCell(parentCell)
{
	surfaceArea = pCell->GetSurfaceArea();
	neuralNet = pCell->GetNeuralNetwork();

	dna->SetCurrentPosition(startingPos);

	for (int i = 0; i < contains_amount; i++)
	{
		outputNodeArr[i] = dna->GetGeneInt(0, pCell->GetNeuralNetwork()->GetOutputLayerCount());
		modifierArrCellToChunk[i] = pow(dna->GetGeneFloat(0, 2), 2);
		modifierArrChunkToCell[i] = pow(dna->GetGeneFloat(0, 2), 2);
		modifierArrPassiveMovement[i] = dna->GetGeneFloat(0, 1);
	}

	pCell->SetMembraneStatus(true);
}

float Membrane::Tick(int t)
{

	totalATPCost = pCell->GetChemCon()->DiffuseFromAndTo(pCell->GetCurrentChunk()->GetChemCon(), t, this);

	return totalATPCost;
}

string Membrane::GetOutputString()
{
	string buffer = " Membrane:\n";
	buffer += "  LastATPCost: " + to_string(totalATPCost) + "\n";

	for (int i = 0; i < contains_amount; i++)
	{
		buffer += "  " + writtenSubstances[i] + " " + to_string(modifierArrCellToChunk[i]) + "/" + to_string(modifierArrChunkToCell[i]) + " (" + to_string((outputNodeArr[i])) + "/" + to_string(pCell->GetNeuralNetwork()->GetOutputNode(outputNodeArr[i])) + ")\n";
	}

	buffer += "\n";

	return buffer;
}


Membrane::~Membrane()
{
}
