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
	dna->SetCurrentPosition(startpos);

	surface = pCell->GetSurfaceArea() / 10.0f;

	buildingOutputNode = dna->GetGeneInt(0, neuralNet->GetOutputLayerCount());

	randomSpawnChance = dna->GetGeneInt(10.000, 1000000);

	pCell->AddToDNAColourZ(randomSpawnChance / 1000000);

	createdThroughDNA = true;
}

SplittingManager::SplittingManager(Cell* parentCell)
{
	pCell = parentCell;
	this->dna = pCell->GetDNA();
	neuralNet = pCell->GetNeuralNetwork();

	surface = pCell->GetSurfaceArea() / 10.0f;

	createdThroughDNA = false;
}

float SplittingManager::Tick(int t)
{
	float ATPchange = 0.0f;

	if (createdThroughDNA)
	{
		if (!splittingCell)
		{
			if (rand() % randomSpawnChance == 0 || neuralNet->GetOutputNode(splittingOutputNode) >= 0.90 || forceSplit)
			{
				forceSplit = false;
				splittingCell = new Cell(pCell->GetRenderClass(), pCell->GetWorld(), dna->CloneDNA());
				totalCost = splittingCell->GetBuildingCost();
			}
		}
		else
		{
			ATPchange = pCell->LimitATPUsage(t * neuralNet->GetUnsignedOutputNode(buildingOutputNode) * Building_Coeffiecent, surface);

			if (splittingCell->BuildCell(ATPchange))
			{
				splittingCell->ReleaseCell(pCell);
				splittingCell->SetPosition(pCell->GetPosition());
				splittingCell->AddPosition(splittingCell->GetSize() / 3, splittingCell->GetSize() / 3, splittingCell->GetSize() / 3);
				pCell->AddPosition(-pCell->GetSize() / 3, -pCell->GetSize() / 3, -pCell->GetSize() / 3);
				pCell->GetWorld()->AddCell(splittingCell);
				splittingCell = nullptr;
				pCell->GetWorld()->IncreaseCellsCreated();
			}
		}
	}
	else
	{
		if (!splittingCell)
		{
			if (rand() % randomSpawnChance == 0 || forceSplit)
			{
				forceSplit = false;
				splittingCell = new Cell(pCell->GetRenderClass(), pCell->GetWorld(), dna->CloneDNA(), pCell);
				totalCost = splittingCell->GetBuildingCost();
			}
		}
		else
		{
			ATPchange = pCell->LimitATPUsage(t  * 0.2f * Building_Coeffiecent, surface);

			if (splittingCell->BuildCell(ATPchange))
			{
				splittingCell->ReleaseCell(pCell);
				splittingCell->SetPosition(pCell->GetPosition());
				splittingCell->AddPosition(splittingCell->GetSize() / 10 + (rand() % 10) / 80, splittingCell->GetSize() / 10 + (rand() % 10) / 80, splittingCell->GetSize() / 10 + (rand() % 10) / 80);
				pCell->AddPosition(-pCell->GetSize() / 10 - (rand() % 10) / 80, -pCell->GetSize() / 10 - (rand() % 100), -pCell->GetSize() / 10 - (rand() % 10));
				pCell->GetWorld()->AddCell(splittingCell);
				splittingCell = nullptr;
				pCell->GetWorld()->IncreaseCellsCreated();
			}
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

	if (createdThroughDNA)
		buffer += "  OutputNode: " + to_string(buildingOutputNode) + " InputNode: " + to_string(buildingInputNode)
			+ "\n        " + to_string(neuralNet->GetUnsignedOutputNode(buildingOutputNode)) + "\n";
	else 
		buffer += "  Not DNA Induced\n";

	return buffer;
}


SplittingManager::~SplittingManager()
{
}
