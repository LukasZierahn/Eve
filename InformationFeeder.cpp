#include "InformationFeeder.h"
#include "Cell.h"
#include "NeuralNetwork.h"
#include "ChemicalContainer.h"
#include "DNA.h"


InformationFeeder::InformationFeeder(Cell* parentCell, DNA* dna, int startingPos)
{
	pCell = parentCell;
	neuralNet = pCell->GetNeuralNetwork();
	chemCon = pCell->GetChemCon();

	dna->SetCurrentPosition(startingPos);

	temperatureInputNode = dna->GetGeneInt(0, neuralNet->GetInputLayerCount());
	for (int i = 0; i < contains_amount; i++)
	{
		containsInputNode[i] = dna->GetGeneInt(0, neuralNet->GetInputLayerCount());
	}
}

void InformationFeeder::InputValuesToNeuralNetwork()
{
	neuralNet->SetInputNode(temperatureInputNode, min(chemCon->GetTemperature() / Max_Temperature, 1.0f));
	for (int i = 0; i < contains_amount; i++)
	{
		neuralNet->SetInputNode(containsInputNode[i], min(chemCon->GetContains(i) / Max_Amount_Of_Substances, 1.0f));
	}
}


InformationFeeder::~InformationFeeder()
{
}
