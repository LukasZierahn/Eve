#include "EnergyManagerOxygen.h"
#include "Cell.h"
#include "Model.h"
#include "DNA.h"
#include "NeuralNetwork.h"
#include "ChemicalContainer.h"

EnergyManagerOxygen::EnergyManagerOxygen(Cell * parentCell, DNA * dna, int startpos)
{
	pCell = parentCell;
	neuralNet = pCell->GetNeuralNetwork();

	dna->SetCurrentPosition(startpos);

	conversionNeuralNode = dna->GetGeneInt(0, neuralNet->GetOutputLayerCount());
	conversionCapabilities = dna->GetGeneFloat(0, 1);

	pCell->AddToDNAColourZ(Filter_EnergyManager, 0.5 + (conversionCapabilities / 2));
}

float EnergyManagerOxygen::Tick(int t)
{
	float modifier = 0.0f;
	double conversionATP = 0.0f;
	float limitingFactorFood = min(2000.0f * pCell->GetChemCon()->GetContains(FOOD_CHEMCON_ID) / pCell->GetVolume(), 1.0f);
	float limitingFactorOxygen = min(2000.0f * pCell->GetChemCon()->GetContains(OXYGEN_CHEMCON_ID) / pCell->GetVolume(), 1.0f);

	conversionATP = min(pCell->LimitATPUsage(neuralNet->GetUnsignedOutputNode(conversionNeuralNode) * conversionCapabilities * pCell->GetSurfaceArea() * limitingFactorFood * limitingFactorOxygen * Conversion_Constant_Oxygen * t, pCell->GetChemCon()->GetVolume(), &modifier), pCell->GetChemCon()->GetContains(FOOD_CHEMCON_ID));

	pCell->GetChemCon()->AddSubstanceToContains(FOOD_CHEMCON_ID, -min(conversionATP * modifier, pCell->GetChemCon()->GetContains(FOOD_CHEMCON_ID)));
	pCell->GetChemCon()->AddSubstanceToContains(OXYGEN_CHEMCON_ID, 6 * -min(conversionATP * modifier, pCell->GetChemCon()->GetContains(OXYGEN_CHEMCON_ID)));

	conversionATP -= (conversionCapabilities * pCell->GetSurfaceArea() * Conversion_Upkeep_Oxygen) / 32;

	return conversionATP * 32;
}

string EnergyManagerOxygen::GetOutputString()
{
	string buffer = "\n Oxygen Energy Manager\n";

	buffer += "  CC: " + to_string(conversionCapabilities);

	buffer += "\n  Output Node: " + to_string(conversionNeuralNode) + " " + to_string(neuralNet->GetUnsignedOutputNode(conversionNeuralNode)) + "\n";

	return buffer;
}

EnergyManagerOxygen::~EnergyManagerOxygen()
{
}
