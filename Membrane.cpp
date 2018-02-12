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

	swellingModifier = dna->GetGeneFloat(0, 1);
	swellingOutputNode = dna->GetGeneFloat(0, pCell->GetNeuralNetwork()->GetOutputLayerCount());

	pCell->SetMembraneStatus(true);
}

float Membrane::Tick(int t)
{
	totalATPCost = pCell->GetChemCon()->DiffuseFromAndTo(pCell->GetCurrentChunk()->GetChemCon(), t, this);

	//swelling
	float modifier = 0.0f;
	totalATPCost += pCell->LimitATPUsage(neuralNet->GetOutputNode(swellingOutputNode) * t * swellingModifier * surfaceArea, surfaceArea, &modifier);

	pCell->AddToSwellPercent(modifier * neuralNet->GetOutputNode(swellingOutputNode) * t * swellingModifier * surfaceArea / pCell->GetChemCon()->GetVolume());

	return totalATPCost;
}

string Membrane::GetOutputString()
{
	string buffer = " Membrane:\n";
	buffer += "  LastATPCost: " + to_string(totalATPCost) + "\n";
	buffer += "  Swelling modifier: " + to_string(swellingModifier) + "\n";
	buffer += "  Swelling OutputNode: " + to_string(swellingOutputNode) + "/" + to_string(neuralNet->GetOutputNode(swellingOutputNode)) + "\n";

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
