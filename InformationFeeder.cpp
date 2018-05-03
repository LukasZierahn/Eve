#include "InformationFeeder.h"
#include "Cell.h"
#include "NeuralNetwork.h"
#include "ChemicalContainer.h"
#include "DNA.h"


InformationFeeder::InformationFeeder(Cell* parentCell, DNA* dna, int startingPos)
{
	pCell = parentCell;
	neuralNet = pCell->GetNeuralNetwork();

	dna->SetCurrentPosition(startingPos);

	//temperatureInputNode = dna->GetGeneInt(0, neuralNet->GetInputLayerCount());
	ATPInputNode = dna->GetGeneInt(0, neuralNet->GetInputLayerCount());
	for (int i = 0; i < contains_amount; i++)
	{
		containsInputNode[i] = dna->GetGeneInt(0, neuralNet->GetInputLayerCount());
	}
}

void InformationFeeder::InputValuesToNeuralNetwork()
{
	ChemicalContainer* chemCon = pCell->GetChemCon();

	//neuralNet->SetInputNode(temperatureInputNode, min(chemCon->GetTemperature() / Max_Temperature, 1.0f));
	for (int i = 0; i < contains_amount; i++)
	{
		neuralNet->SetInputNode(containsInputNode[i], min(chemCon->GetContains(i) / (Max_Amount_Of_Substances * chemCon->GetVolume()), 1.0f), i + Neural_InpNode_Source_ContainsOffset);
	}
	neuralNet->SetInputNode(ATPInputNode, min(pCell->GetATP() / (Max_ATP * chemCon->GetVolume()), 1.0f), Neural_InpNode_Source_ATP);
}


InformationFeeder::~InformationFeeder()
{
}
