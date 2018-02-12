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

	createdThroughDNA = true;
}

EnergyManager::EnergyManager(Cell * parentCell)
{
	pCell = parentCell;
	neuralNet = pCell->GetNeuralNetwork();
	chemCon = pCell->GetChemCon();

	conversionCapabilities = 0.2f;
	conversionNeuralNode = -1;
}

float EnergyManager::Tick(int t)
{
	float modifier = 0.0f;
	double conversionATP = 0.0f;
	if (createdThroughDNA)
		conversionATP = pCell->LimitATPUsage(neuralNet->GetUnsignedOutputNode(conversionNeuralNode) * conversionCapabilities * pCell->GetSurfaceArea() * chemCon->GetContains(FOOD_CHEMCON_ID) * CONVERSION_CONSTANT * t, chemCon->GetVolume(), &modifier);
	else
		conversionATP = pCell->LimitATPUsage(conversionCapabilities * pCell->GetSurfaceArea() * chemCon->GetContains(FOOD_CHEMCON_ID) * CONVERSION_CONSTANT * t, chemCon->GetVolume(), &modifier);


	double ATPChange = min(conversionATP * modifier, chemCon->GetContains(FOOD_CHEMCON_ID)) * 2;
	chemCon->AddSubstanceToContains(FOOD_CHEMCON_ID, -min(conversionATP * modifier, chemCon->GetContains(FOOD_CHEMCON_ID)));

	ATPChange -= conversionCapabilities * pCell->GetSurfaceArea() * CONVERSION_UPKEEP;

	return ATPChange;
}

string EnergyManager::GetOutputString()
{
	string buffer = "\n EnergyManager\n";

	buffer += "  CC: " + to_string(conversionCapabilities);
	if (conversionNeuralNode != -1)
	{
		buffer += "\n  Output Node: " + to_string(conversionNeuralNode) + "\n        "
			+ to_string(neuralNet->GetUnsignedOutputNode(conversionNeuralNode)) + "\n";
	}
	else
	{
		buffer += "\n  Not DNA Induced\n";
	}

	return buffer;
}


EnergyManager::~EnergyManager()
{
}
