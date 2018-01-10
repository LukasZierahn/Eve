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

	for (int i = 0; i < contains_amount; i++)
	{
		outputNodeArr[i] = dna->GetCharacter(startingPos + i * 3) % pCell->GetNeuralNetwork()->GetOutputLayerCount();
		modifierArr[i] = pow((1.0f * dna->GetCharacter(startingPos + i * 3 + 1) + dna->GetCharacter(startingPos + i * 3 + 2)) / UniqCharsInDNA, 2);
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
	buffer += "  LastATPCost:" + to_string(totalATPCost) + "\n";

	for (int i = 0; i < contains_amount; i++)
	{
		buffer += "  " + writtenSubstances[i] + " " + to_string(modifierArr[i]) + " (" + to_string((outputNodeArr[i])) + "/" + to_string(pCell->GetNeuralNetwork()->GetOutputNode(outputNodeArr[i])) + ")\n";
	}

	buffer += "\n";

	return buffer;
}


Membrane::~Membrane()
{
}
