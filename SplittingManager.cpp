#include "SplittingManager.h"
#include "NeuralNetwork.h"
#include "DNA.h"
#include "Cell.h"
#include "World.h"

void SplittingManager::SplitCellOff(int t)
{
	lastCheckedForSpace += t;
	if (lastCheckedForSpace >= 200 && pCell->GetSquaredDistanceToClosestCell() >= pow((pCell->GetLength() + pCell->GetSize()) / 10.0f, 2))
	{
		lastCheckedForSpace = 0;
		splittingCell->ReleaseCell(pCell);
		splittingCell->SetPosition(pCell->GetPosition());

		float x = rand() / (1.0f * RAND_MAX);
		float y = (rand() / (1.0f * RAND_MAX)) * (sqrt(1 - pow(x, 2)));
		float z = sqrt(1 - pow(x, 2) - pow(y, 2));

		x /= 10.0f;
		y /= 10.0f;
		z /= 10.0f;

		splittingCell->AddPosition(splittingCell->GetSize()  * x, splittingCell->GetSize() * y, splittingCell->GetSize() * z);
		pCell->AddPosition(pCell->GetSize() * -x, pCell->GetSize() * -y, pCell->GetSize() * -z);
		pCell->GetWorld()->AddCell(splittingCell);
		splittingCell = nullptr;
		pCell->GetWorld()->IncreaseCellsCreated();
	}

}

SplittingManager::SplittingManager(Cell* parentCell, DNA* dna, int startpos)
{
	pCell = parentCell;
	neuralNet = pCell->GetNeuralNetwork();
	this->dna = dna;
	dna->SetCurrentPosition(startpos);

	surface = pCell->GetSurfaceArea() / 10.0f;

	splittingOutputNode = dna->GetGeneInt(0, neuralNet->GetOutputLayerCount());
	buildingOutputNode = dna->GetGeneInt(0, neuralNet->GetOutputLayerCount());

	randomSpawnChance = dna->GetGeneInt(100, 800000);
	buildingSpeed = dna->GetGeneFloat(0.02, 4);

	pCell->AddToDNAColourZ(Filter_SplittingMembrane, 0.5 + (randomSpawnChance / 2000000.0f));

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
			ATPchange = pCell->LimitATPUsage(t * neuralNet->GetUnsignedOutputNode(buildingOutputNode) * buildingSpeed * Building_Coeffiecent, surface);
			if (splittingCell->BuildCell(ATPchange))
			{
				SplitCellOff(t);
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
			ATPchange = pCell->LimitATPUsage(t * buildingSpeed * Building_Coeffiecent, surface);
			if (splittingCell->BuildCell(ATPchange))
			{
				SplitCellOff(t);
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

	buffer += "  Random Split Chance: " + to_string(randomSpawnChance) + "\n";

	if (createdThroughDNA)
		buffer += "  OutputNode: " + to_string(buildingOutputNode) + " " + to_string(neuralNet->GetUnsignedOutputNode(buildingOutputNode))
		+ "\n  InputNode: " + to_string(buildingInputNode) + " " + to_string(neuralNet->GetInputNode(buildingInputNode)) + "\n";
	else
		buffer += "  Not DNA Induced\n";

	return buffer;
}

void SplittingManager::InputValuesToNeuralNetwork()
{
	if (splittingCell)
		neuralNet->SetInputNode(buildingInputNode, 1, Neural_InpNode_Source_Splitting);
	else
		neuralNet->SetInputNode(buildingInputNode, 0, Neural_InpNode_Source_Splitting);
}


SplittingManager::~SplittingManager()
{
}
