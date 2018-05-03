#include "EnergyManager.h"
#include "Cell.h"
#include "Model.h"
#include "DNA.h"
#include "NeuralNetwork.h"
#include "ChemicalContainer.h"
#include "RenderClass.h"


EnergyManager::EnergyManager(Cell* parentCell, DNA* dna, int startpos)
{
	pCell = parentCell;
	neuralNet = pCell->GetNeuralNetwork();

	dna->SetCurrentPosition(startpos);

	conversionNeuralNode = dna->GetGeneInt(0, neuralNet->GetOutputLayerCount());
	conversionCapabilities = dna->GetGeneFloat(0, 1);

	pCell->AddToDNAColourX(Filter_EnergyManager, 0.5 + (conversionCapabilities / 2));

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
	float limitingFactorFood = min(2000.0f * pCell->GetChemCon()->GetContains(FOOD_CHEMCON_ID) / pCell->GetVolume() , 1.0f);

	if (createdThroughDNA)
		conversionATP = min(pCell->LimitATPUsage(neuralNet->GetUnsignedOutputNode(conversionNeuralNode) * conversionCapabilities * pCell->GetSurfaceArea() * limitingFactorFood * CONVERSION_CONSTANT * t, pCell->GetChemCon()->GetVolume(), &modifier), pCell->GetChemCon()->GetContains(FOOD_CHEMCON_ID));
	else
		conversionATP = min(pCell->LimitATPUsage(conversionCapabilities * pCell->GetSurfaceArea() * limitingFactorFood * CONVERSION_CONSTANT * t, pCell->GetChemCon()->GetVolume(), &modifier), pCell->GetChemCon()->GetContains(FOOD_CHEMCON_ID));


	pCell->GetChemCon()->AddSubstanceToContains(FOOD_CHEMCON_ID, -min(conversionATP * modifier, pCell->GetChemCon()->GetContains(FOOD_CHEMCON_ID)));

	conversionATP -= (conversionCapabilities * pCell->GetSurfaceArea() * CONVERSION_UPKEEP) / 2;

	return conversionATP * 2;
}

string EnergyManager::GetOutputString()
{
	string buffer = "\n Energy Manager\n";

	buffer += "  CC: " + to_string(conversionCapabilities);
	if (conversionNeuralNode != -1)
	{
		buffer += "\n  Output Node: " + to_string(conversionNeuralNode) + " " + to_string(neuralNet->GetUnsignedOutputNode(conversionNeuralNode)) + "\n";
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
