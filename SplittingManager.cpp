#include "SplittingManager.h"
#include "NeuralNetwork.h"
#include "DNA.h"
#include "Cell.h"
#include "World.h"

SplittingManager::SplittingManager(Cell* parentCell, DNA* dna, int startpos)
{
	pCell = parentCell;
	neuralNet = pCell->GetNeuralNetwork();
	this->dna = dna;

	buildingOutputNode = dna->GetCharacter(startpos) % neuralNet->GetOutputLayerCount();
	buildingOutputNode = dna->GetCharacter(startpos + 1) % neuralNet->GetOutputLayerCount();
}

float SplittingManager::Tick(int t)
{
	float ATPchange = 0.0f;

	if (!splittingCell)
	{
		if (rand() % 10000 == 0)
		{
			splittingCell = new Cell(pCell->GetRenderClass(), pCell->GetWorld(), dna->CloneDNA());
			totalCost = splittingCell->GetBuildingCost();
		}
	}
	else
	{
		ATPchange = pCell->GetATP() * neuralNet->GetOutputNode(buildingOutputNode) * t;

		if (splittingCell->BuildCell(pCell->GetATP() * neuralNet->GetOutputNode(buildingOutputNode) * t))
		{
			splittingCell->SetPosition(pCell->GetPosition());
			splittingCell->AddPosition(2, 0, 0);
			pCell->GetWorld()->AddCell(splittingCell);
			splittingCell = nullptr;
		}
	}

	return -ATPchange;
}

string SplittingManager::GetOutputString()
{
	string buffer = "\n SplittingManager: \n";

	if (splittingCell)
	{
		buffer += "  Progress: " + to_string(splittingCell->GetBuildingCost()) + "/" + to_string(totalCost) + "\n";
	}
	else
	{
		buffer += "  Cell is currently not splitting \n";
	}

	buffer += "  OutputNode: " + to_string(buildingOutputNode) + " InputNode: " + to_string(buildingInputNode)
		+ "\n        " + to_string(neuralNet->GetOutputNode(buildingOutputNode));

	return buffer;
}


SplittingManager::~SplittingManager()
{
}
