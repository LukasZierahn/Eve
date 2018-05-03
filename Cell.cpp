#include "Cell.h"
#include "Model.h"
#include "RenderClass.h"
#include "include.h"
#include "ModelLoader.h"
#include "ChemicalContainer.h"
#include "World.h"
#include "Chunk.h"
#include "DNA.h"
#include "NeuralNetwork.h"
#include "IncludeTraits.h"
#include "EnergyManagerOxygen.h"

const string Cell::dnaCriteria[] = { "Fl", "Mm", "i", "e", "s", "Ex"};

Cell::Cell(RenderClass* rndCls, World* world, DNA* InpDNA, Cell* pCell) : render(rndCls)
{
	ID = world->GetNextID();
	velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	filterColourCount = Filter_Total_Count;
	filterColour = new XMFLOAT4[filterColourCount];
	ZeroMemory(filterColour, sizeof(XMFLOAT4) * filterColourCount);

	dna = InpDNA;
	if (InpDNA == nullptr)
	{
		dna = new DNA();
		dna->GenerateRandomDNA(60);
	}
	dna->SetCurrentPosition(0);

	OutputDebugString((dna->GetString() + "\n").c_str());

	chunkSize = world->GetChunkSize();

	size = dna->GetGeneFloat(0.1, 10);
	length = dna->GetGeneFloat(0, 10);

	//all cells are oblate Spheres as length is >= 0

	float e = sqrt(1 - (pow(size, 2) / pow(size + length, 2)));
	surfaceArea = XM_PI * (2 * pow(size + length, 2) + pow(size, 2) * log((1 + e) / (1 - e)) / e);
	volume = 4/3 * (size + length) * pow(size, 2) * XM_PI;

	oxygenSusceptiblity = max(0.0f, dna->GetGeneFloat(-1.0f, 10.0f));

	mod = new Model(rndCls);
	mod->SetScale(size + length, size, size);
	this->world = world;

	if (pCell)
	{
		parentID = pCell->ID;
		chemCon = new ChemicalContainer(world, volume, surfaceArea);
		this->generation = pCell->generation + 1;
	}
	else
	{
		chemCon = new ChemicalContainer(world, volume, surfaceArea);
		this->generation = 1;
		ATP = volume * 5; //give a new created some ATP to get going
	}

	rndCls->GetModelLoader()->GetModel("Sphere", mod->GetDataPointer());
	rndCls->GetModelLoader()->GetTexture("Cell", mod->GetTexturePointer());
	rndCls->GetModelLoader()->GetTexture("CellFilter", mod->GetFilterTexturePointer());

	neuralNet = new NeuralNetwork();
	neuralNet->BuildFromDNA(dna, 11);

	//AddToDNAColourX((neuralNet->GetInputLayerCount() + neuralNet->GetHiddenLayerCount() + neuralNet->GetOutputLayerCount()) / (1.0f * (Max_Input_Nodes + Max_Hidden_Nodes + Max_Ouput_Nodes)));

	CheckDNAForTraits();

	for (Trait* trt : traits)
	{
		buildingCost += trt->GetATPBuildingCost();
	}

	buildingCost += dna->GetString().size() * BuildingCost_DNA_Factor;
	buildingCost += volume * (10.0f - oxygenSusceptiblity) * BuildingCost_OxygenSusceptibility_Factor;
	buildingCost *= BuildingCost_Factor;

	initialBuildingCost = buildingCost;
}

Cell::Cell(RenderClass* rndCls, World* world, DNA* dna, float x, float y, float z) : Cell(rndCls, world, dna)
{
	SetPosition(x, y, z);
}

void Cell::Tick(float t)
{
	timeAlive += t;

	XMFLOAT4 pos = *mod->GetPosition();
	chunk = world->GetChunk(floor(pos.x / chunkSize), floor(max(pos.y / chunkSize, 49)), floor(pos.z / chunkSize));

	for (NeuralNetworkInput* nI : neuralInps)
	{
		nI->InputValuesToNeuralNetwork();
	}

	ATP -= neuralNet->ComputeResult() * t;

	if (!hasMembrane)
		chemCon->DiffuseFromAndTo(chunk->GetChemCon(), t, surfaceArea);

	for (Trait* trt : traits)
	{
		ATP += trt->Tick(t);
	}


	chemCon->ApplyContains();
	chunk->GetChemCon()->ApplyContains();

	//attrition through everything really
	ATP -= t * volume / 500000.0f;

	//poison stuff
	float poisonAffectingCell = (chunk->GetChemCon()->GetContains(POISON_CHEMCON_ID) * t) / 5.0f + (chemCon->GetContains(POISON_CHEMCON_ID) * t) * 10.0f;
	ATP -= poisonAffectingCell;
	totalPoisonDamage += poisonAffectingCell;

	ATP -= oxygenSusceptiblity * chunk->GetChemCon()->GetContains(OXYGEN_CHEMCON_ID) * Oxygen_Damage_Factor;

	bool mutated = false;
	if (poisonAffectingCell >= 0.001)
		mutated = dna->MutateDNAThroughPoison(poisonAffectingCell);

	//if the DNA mutates, we have to redo the whole cell, maybe there is a less CPU demanding way to do this, i will think about that should it become a problem
	if (mutated)
	{
		OutputDebugString("DNA got Damaged!\n");

		for (NeuralNetworkInput* nI : neuralInps)
		{
			if (nI->GetType() == Type_InformationFeeder)
			{
				delete nI;
			}
		}

		for (Trait* t : traits)
		{
			delete t;
		}

		traits.clear();
		neuralInps.clear();


		dna->SetCurrentPosition(0);

		size = dna->GetGeneFloat(0.1, 10);
		length = dna->GetGeneFloat(0, 10);

		float e = sqrt(1 - (pow(size, 2) / pow(size + length, 2)));
		surfaceArea = XM_PI * (2 * pow(size + length, 2) + pow(size, 2) * log((1 + e) / (1 - e)) / e);
		volume = 4 / 3 * (size + length) * pow(size, 2) * XM_PI;

		ZeroMemory(filterColour, sizeof(XMFLOAT4) * filterColourCount);

		bool hasMembrane = false;
		bool hasEnergyManager = false;
		bool hasSplitter = false;

		CheckDNAForTraits();
	}

	//swelling stuff
	swellPercent += chemCon->GetSwellAmount(chunk->GetChemCon()) * t / volume;
	swellPercent += ATP_Swelling_Factor * ATP * t / volume;

	if (swellPercent >= 1.25 || swellPercent <= 0.75 || ATP <= 0)
	{
		Die(swellPercent >= 1.25);
	}

	velocity.x *= pow(0.99, t / 10);
	velocity.y *= pow(0.99, t / 10);
	velocity.z *= pow(0.99, t / 10);
	
	if (isnan(ATP) || isinf(ATP)
		|| isnan(swellPercent) || isinf(swellPercent))
	{
		OutputDebugString("oh no");
		Die(false, true);
	}

	float velocityLength = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));

	if (velocityLength >= 0.001)
		mod->SetRotation(0, asin((velocity.x / velocityLength)) + XM_PI, asin(velocity.y / velocityLength));

	mod->AddPosition(velocity.x, velocity.y, velocity.z);
	mod->SetScale(swellPercent * (size + length), size * swellPercent, size * swellPercent);

	mod->SetFilterColour(filterColour[render->GetCellDisplayMode()]);

	if (render->GetCellDisplayMode() == Filter_CellHealth)
		mod->SetFilterColour(XMFLOAT4((swellPercent - 1) / 0.25, 0.0f, -min(ATP / 3000.0f, 1.0f), 0.0f));

}

void Cell::CheckDNAForTraits()
{
	for (int i = 0; i < Type_Absolute_Amount; i++)
	{
		int searchStart = 0;
		while (searchStart != string::npos)
		{
			searchStart = dna->GetString().find(dnaCriteria[i], searchStart);
			if (searchStart != string::npos)
			{
				searchStart++;

				if (i == Type_Flagellum)
				{
					traits.push_back(new Flagellum(this, dna, searchStart));
				}

				if (i == Type_Membrane)
				{
					traits.push_back(new Membrane(this, dna, searchStart));
					hasMembrane = true;
					break;
				}		
			
				if (i == Type_InformationFeeder)
				{
					neuralInps.push_back(new InformationFeeder(this, dna, searchStart));
					break; //only one InformationFeeder should be created since they would just override each other and otherwise all "i"'s in the DNA get clustered with new InfFeeder
				}

				if (i == Type_EnergyManager)
				{
					traits.push_back(new EnergyManager(this, dna, searchStart));
					hasEnergyManager = true;
					break;
				}

				if (i == Type_SplittingManager)
				{
					SplittingManager* newSplit = new SplittingManager(this, dna, searchStart);
					traits.push_back(newSplit);
					neuralInps.push_back(newSplit);
					hasSplitter = true;
					break;
				}

				if (i == Type_EnergyManagerOxygen)
				{
					traits.push_back(new EnergyManagerOxygen(this, dna, searchStart));
					hasEnergyManager = true;
					break;
				}
			}
		}
	}

	if (!hasSplitter)
		traits.push_back(new SplittingManager(this));

	if (!hasEnergyManager)
		traits.push_back(new EnergyManager(this));

	traits.shrink_to_fit();

}

void Cell::SetSelected(bool sel)
{
	isSelected = sel;
	if (isSelected)
	{
		render->GetModelLoader()->GetTexture("CellSelected", mod->GetTexturePointer());
	}
	else
	{
		render->GetModelLoader()->GetTexture("Cell", mod->GetTexturePointer());
	}
}

float Cell::LimitATPUsage(float LimitATPUsage, float Surface, float* modifier)
{
	if (LimitATPUsage == 0)
	{
		*modifier = 0;
		return 0.0f;
	}

	if (modifier)
		*modifier = min((ATP * Surface) / (volume * LimitATPUsage), 1.0f);
	return min((ATP * Surface) / (volume * LimitATPUsage), 1.0f) * LimitATPUsage;
}

float Cell::LimitATPUsage(float LimitATPUsage, float Surface)
{
	if (LimitATPUsage == 0)
		return 0.0f;

	return min((ATP * Surface) / (volume * LimitATPUsage), 1.0f) * LimitATPUsage;
}

float Cell::GetSquaredDistanceToClosestCell()
{
	float d = -1.0f;
	float cd = 0;
	bool skippedMyself = false;

	for (Cell* c : *world->GetCellVec())
	{
		cd = pow(c->GetPositionX() - mod->GetPosition()->x, 2) + pow(c->GetPositionY() - mod->GetPosition()->y, 2) + pow(c->GetPositionZ() - mod->GetPosition()->z, 2);
		if (d == -1 || cd < d)
		{
			if (!skippedMyself && cd == 0)
			{
				skippedMyself = true;
				continue;
			}

			d = cd;
		}
	}

	return d;
}

void Cell::SetPosition(float x, float y, float z)
{
	int worldChunkSize = world->GetChunkSize();
	world->KeepPointInBounds(&x, &y, &z);

	mod->SetPosition(x, y, z);

}

void Cell::AddPosition(float x, float y, float z)
{
	XMFLOAT4 pos = *mod->GetPosition();

	x += pos.x;
	y += pos.y;
	z += pos.z;

	int worldChunkSize = world->GetChunkSize();
	world->KeepPointInBounds(&x, &y, &z);

	mod->SetPosition(x, y, z);
}

SplittingManager* Cell::GetSplittingManager()
{
	for (Trait* t : traits)
	{
		if (t->GetType() == Type_SplittingManager)
		{
			return static_cast<SplittingManager*>(t);
		}
	}
}

void Cell::ReleaseCell(Cell* pCell)
{
	chemCon->GetContainsFromParentCell(pCell->chemCon);
	pCell->ATP = pCell->ATP / 2;
	ATP = pCell->ATP;
}

void Cell::Die(bool explode, bool faulty)
{

	if (ATP <= 0)
	{
		if (GetSplittingManager())
			world->IncreaseDeathByATPLack(GetSplittingManager()->IsSplitting());
		else
			world->IncreaseDeathByATPLack(false);
	}
	else
	{
		world->IncreaseDeathBySwelling();
	}

	if (!faulty)
	{
		for (int i = 0; i < contains_amount; i++)
		{
			chunk->GetChemCon()->AddSubstanceToContains(i, chemCon->GetContains(i));
		}

		if (explode)
			chunk->GetChemCon()->AddSubstanceToContains(POISON_CHEMCON_ID, volume / 50000); //poison
		else
			chunk->GetChemCon()->AddSubstanceToContains(POISON_CHEMCON_ID, volume / 100000);
	}
	else
	{
		world->IncreaseFaultyCells();
	}

	isAlive = false;
	world->RemoveCell(ID);
}

void Cell::ForceSplit()
{
	for (Trait* trt : traits)
	{
		if (trt->GetType() == Type_SplittingManager)
			static_cast<SplittingManager*>(trt)->StartSplitting();
	}
}

string Cell::GetOutputString()
{
	XMFLOAT4 pos = *mod->GetPosition();
	string buffer = "";

	buffer += " Cell ID: " + to_string(ID) + "\n";
	buffer += " Cell Generation: " + to_string(generation) + "\n";
	buffer += " Cell is Alive: " + to_string(isAlive) + "\n";
	buffer += " Cell Alive Time: " + to_string(timeAlive) + "\n";
	buffer += " Cell Position: " + to_string(pos.x) + "/" + to_string(pos.y) + "/" + to_string(pos.z) + "\n";
	buffer += " Cell Speed: " + to_string(velocity.x) + "/" + to_string(velocity.y) + "/" + to_string(velocity.z) + "\n";
	buffer += " Cell Volume/surface: " + to_string(volume) + "/" + to_string(surfaceArea) + "\n";
	buffer += " Cell size/length: " + to_string(size) + "/" + to_string(length) + "\n";
	buffer += " Cell Swell Percent: "  + to_string(swellPercent) + "\n";
	buffer += " Cell ATP: " + to_string(ATP) + "\n\n";

	buffer += " Cell Building Cost: " + to_string(initialBuildingCost) + "\n";
	buffer += " Cell Total Poison Damage: " + to_string(totalPoisonDamage) + "\n\n";
	buffer += " Cell Oxygen Susceptibility: " + to_string(oxygenSusceptiblity) + "\n\n";

	buffer += " Input Nodes: ";
	for (int i = 0; i < neuralNet->GetInputLayerCount(); i++)
	{
		if (i % 3 == 2)
		{
			buffer += "\n       ";
		}
		buffer += to_string(neuralNet->GetInputNode(i)) + " ";
	}
	buffer += "\n";

	buffer += " Hidden layer count: " + to_string(neuralNet->GetHiddenLayerCount()) + "\n";

	buffer += " Output Nodes: ";
	for (int i = 0; i < neuralNet->GetOutputLayerCount(); i++)
	{
		if (i % 3 == 2)
		{
			buffer += "\n       ";
		}
		buffer += to_string(neuralNet->GetOutputNode(i)) + " ";
	}
	buffer += "\n\n";

	buffer += " Cell contains: \n";

	//buffer += "  Temperature: " + to_string(chemCon->GetTemperature()) + "\n";
	for (int i = 0; i < contains_amount; i++)
	{
		buffer += "  " + writtenSubstances[i] + ": " + to_string(chemCon->GetContains()[i]) + "\n";
	}

	buffer += "\n Chunk (" + to_string(chunk->GetX()) + "/" + to_string(chunk->GetY()) + "/" + to_string(chunk->GetZ()) + ") contains: \n";

	//buffer += "  Temperature: " + to_string(chunk->GetChemCon()->GetTemperature()) + "\n";
	for (int i = 0; i < contains_amount; i++)
	{
		buffer += "  " + writtenSubstances[i] + ": " + to_string(chunk->GetChemCon()->GetContains()[i]) + "\n";
	}

	buffer += "\n Traits:";
	for (Trait* trt : traits)
	{
		buffer += trt->GetOutputString();
	}

	buffer += "\n\n DNA:\n ";

	buffer += dna->GetString();

	return buffer;
}

void Cell::AddCountForOutput(CellInformation* cellInfo)
{
	{
		for (Trait* trt : traits)
		{
			if(trt->GetType() != Type_EnergyManager && trt->GetType() != Type_SplittingManager)
				cellInfo->traitCount[trt->GetType()] += 1;

			if (trt->GetType() == Type_EnergyManager)
			{
				if (static_cast<EnergyManager*>(trt)->GetDNAInduced())
				{
					cellInfo->traitCount[Type_EnergyManager] += 1;
					cellInfo->energyManagerCC += static_cast<EnergyManager*>(trt)->GetConversionCapabilities();
				}
			}

			if (trt->GetType() == Type_SplittingManager)
			{
				if (static_cast<SplittingManager*>(trt)->GetDNAInduced())
				{
					cellInfo->traitCount[Type_SplittingManager] += 1;
					cellInfo->splittingManagerPotenz += static_cast<SplittingManager*>(trt)->GetRandomSpawnChance();
				}
			}

			if (trt->GetType() == Type_EnergyManagerOxygen)
			{
				cellInfo->energyManagerOxygenCC += static_cast<EnergyManagerOxygen*>(trt)->GetConversionCapabilities();
			}

			if (trt->GetType() == Type_Membrane)
			{
				for (int i = 0; i < contains_amount; i++)
				{
					cellInfo->membranePassive[i] += static_cast<Membrane*>(trt)->GetModifierPassiv(i);
					cellInfo->membraneAktiveChunkToCell[i] += static_cast<Membrane*>(trt)->GetModifierChunkToCell(i);
					cellInfo->membraneAktiveCellToChunk[i] += static_cast<Membrane*>(trt)->GetModifierCellToChunk(i);

					if (isnan(cellInfo->membraneAktiveCellToChunk[i]) || isinf(cellInfo->membraneAktiveCellToChunk[i])
						|| isnan(cellInfo->membraneAktiveChunkToCell[i]) || isinf(cellInfo->membraneAktiveChunkToCell[i])
						|| isnan(cellInfo->membranePassive[i]) || isinf(cellInfo->membranePassive[i]))
					{
						OutputDebugString("Oh No");
					}
				}
			}
		}

		for (NeuralNetworkInput* nI : neuralInps)
		{
			if (nI->GetType() != Type_EnergyManager)
			{
				cellInfo->traitCount[nI->GetType()] += 1;
			}
		}
		cellInfo->neuralNetworkNodes[0] += neuralNet->GetInputLayerCount();
		cellInfo->neuralNetworkNodes[1] += neuralNet->GetHiddenLayerCount();
		cellInfo->neuralNetworkNodes[2] += neuralNet->GetOutputLayerCount();
	}

	neuralNet->AddSourcesToCellInformation(cellInfo);
}

Cell::~Cell()
{
	for (Trait* t : traits)
	{
		delete t;
	}

	for (NeuralNetworkInput* nI : neuralInps)
	{
		delete nI;
	}

	delete chemCon, mod, dna, neuralNet;
	delete[] filterColour;
}
