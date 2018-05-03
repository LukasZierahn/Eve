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
		modifierArrCellToChunk[i] = dna->GetGeneFloat(0, 20.0f);
		modifierArrChunkToCell[i] = dna->GetGeneFloat(0, 20.0f);
		modifierArrPassiveMovement[i] = dna->GetGeneFloat(0, 1);
	}

	swellingModifier = dna->GetGeneFloat(0, 1);
	swellingOutputNode = dna->GetGeneInt(0, pCell->GetNeuralNetwork()->GetOutputLayerCount());

	pCell->SetMembraneStatus(true);

	pCell->AddToDNAColourX(Filter_SplittingMembrane, 0.75);
}

float Membrane::Tick(int t)
{
	totalATPCost = pCell->GetChemCon()->DiffuseFromAndTo(pCell->GetCurrentChunk()->GetChemCon(), t, this);

	//swelling
	double swellingChange = pCell->LimitATPUsage(Swell_Speed_Factor * neuralNet->GetOutputNode(swellingOutputNode) * t * swellingModifier * surfaceArea / pCell->GetChemCon()->GetVolume(), surfaceArea);;
	totalATPCost += abs(swellingChange * Swell_Cost_Factor);

	pCell->AddToSwellPercent(swellingChange);


	if (isnan(totalATPCost) || isinf(totalATPCost))
	{
		OutputDebugString("oh no");
	}


	return -1 * totalATPCost * Membrane_ATP_Cost_Modfier;
}

string Membrane::GetOutputString()
{
	string buffer = "\n Membrane:\n";
	buffer += "  LastATPCost: " + to_string(totalATPCost) + "\n";
	buffer += "  Swelling modifier: " + to_string(swellingModifier) + "\n";
	buffer += "  Swelling OutputNode: " + to_string(swellingOutputNode) + "/" + to_string(neuralNet->GetOutputNode(swellingOutputNode)) + "\n\n";

	buffer += "  Passive Transport:\n";

	for (int i = 0; i < contains_amount; i++)
	{
		buffer += "  " + writtenSubstances[i] + " " + to_string(modifierArrPassiveMovement[i]) + "\n";
	}


	buffer += "\n  Active Transport:\n";
	
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
