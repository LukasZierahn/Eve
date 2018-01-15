#include "EnergyManager.h"
#include "Cell.h"
#include "DNA.h"
#include "NeuralNetwork.h"
#include "ChemicalContainer.h"


EnergyManager::EnergyManager(Cell* parentCell, DNA* dna, int startpos)
{
	pCell = parentCell;
	neuralNet = pCell->GetNeuralNetwork();
	chemCon = pCell->GetChemCon();

	dna->SetCurrentPosition(startpos);

	conversionNeuralNode = dna->GetGeneInt(0, neuralNet->GetOutputLayerCount());
	conversionCapabilities = dna->GetGeneFloat(0, 1);
}

float EnergyManager::Tick(int t)
{
	double conversionATP = neuralNet->GetUnsignedOutputNode(conversionNeuralNode) * conversionCapabilities * chemCon->GetContains(FOOD_CHEMCON_ID) * CONVERSION_CONSTANT / chemCon->GetVolume();

	chemCon->AddSubstanceToContains(FOOD_CHEMCON_ID,-conversionATP);

	return conversionATP * CONTAINS_TO_ABSOLUTE_NUMBERS * 2;
}

string EnergyManager::GetOutputString()
{
	string buffer = " EnergyManager\n";

	buffer += "  CC: " + to_string(conversionCapabilities);
	buffer += "\n  Output Nodes: " + to_string(conversionNeuralNode) + "\n        "
		+ to_string(neuralNet->GetUnsignedOutputNode(conversionNeuralNode)) + "\n";

	return buffer;
}


EnergyManager::~EnergyManager()
{
}
