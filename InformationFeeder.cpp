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

	temperatureInputNode = dna->GetCharacter(startingPos + 1) % neuralNet->GetInputLayerCount();
	for (int i = 0; i < contains_amount; i++)
	{
		containsInputNode[i] = dna->GetCharacter(startingPos + i + 1) % neuralNet->GetInputLayerCount();
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
