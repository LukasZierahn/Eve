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

	conversionNeuralNode = dna->GetCharacter(startpos) % neuralNet->GetOutputLayerCount();
	conversionCapabilities = dna->GetCharacter(startpos + 1) * 1.0f / UniqCharsInDNA;
}

float EnergyManager::Tick(int t)
{
	double conversionATP = neuralNet->GetOutputNode(conversionNeuralNode) * conversionCapabilities * chemCon->GetContains(FOOD_CHEMCON_ID) * CONVERSION_CONSTANT / chemCon->GetVolume();

	chemCon->AddSubstanceToContains(FOOD_CHEMCON_ID,-conversionATP);

	return conversionATP * CONTAINS_TO_ABSOLUTE_NUMBERS * 2;
}

string EnergyManager::GetOutputString()
{
	string buffer = " EnergyManager\n";

	buffer += "  CC: " + to_string(conversionCapabilities);
	buffer += "\n  Output Nodes: " + to_string(conversionNeuralNode) + "\n        "
		+ to_string(neuralNet->GetOutputNode(conversionNeuralNode)) + "\n";

	return buffer;
}


EnergyManager::~EnergyManager()
{
}
