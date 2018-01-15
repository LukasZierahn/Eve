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

const string Cell::dnaCriteria[] = { "Fl", "Me", "i", "e", "s" };

Cell::Cell(RenderClass* rndCls, World* world, DNA* InpDNA, Cell* pCell) : render(rndCls)
{
	ID = world->GetNextID();
	velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	dna = InpDNA;
	if (InpDNA == nullptr)
	{
		dna = new DNA();
		dna->GenerateRandomDNA(1000);
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

	mod = new Model(rndCls);
	mod->SetScale(size + length, size, size);
	this->world = world;

	if (pCell)
	{
		parentID = pCell->ID;
		chemCon = new ChemicalContainer(world, volume, surfaceArea, pCell->chemCon);
	}
	else
	{
		chemCon = new ChemicalContainer(world, volume, surfaceArea);
	}


	rndCls->GetModelLoader()->GetModel("Sphere", mod->GetDataPointer());
	rndCls->GetModelLoader()->GetTexture("Cell", mod->GetTexturePointer());

	neuralNet = new NeuralNetwork();
	neuralNet->BuildFromDNA(dna, 11);

	CheckDNAForTraits();

	for (Trait* trt : traits)
	{
		buildingCost += trt->GetATPBuildingCost();
	}
}

Cell::Cell(RenderClass* rndCls, World* world, DNA* dna, float x, float y, float z) : Cell(rndCls, world, dna)
{
	SetPosition(x, y, z);
}

void Cell::Tick(float t)
{
	XMFLOAT4 pos = *mod->GetPosition();
	chunk = world->GetChunk(floor(pos.x / chunkSize), floor(pos.y / chunkSize), floor(pos.z / chunkSize));

	for (NeuralNetworkInput* nI : neuralInps)
	{
		nI->InputValuesToNeuralNetwork();
	}

	neuralNet->ComputeResult();

	if (!hasMembrane)
		chemCon->DiffuseFromAndTo(chunk->GetChemCon(), t);

	for (Trait* trt : traits)
	{
		ATP += trt->Tick(t);
	}

	chemCon->ApplyContains();
	chunk->GetChemCon()->ApplyContains();


	swellPercent += chemCon->GetSwellAmount(chunk->GetChemCon()) * t / volume;

	float velocityLength = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));

	//this is drag by the water
	if (velocityLength != 0)
	{
		velocity.x = velocity.x / (pow(velocityLength, 2) + 1); //if things go slow then and this pow is kept then the sqrt in velocityLength is maybe redundant
		velocity.y = velocity.y / (pow(velocityLength, 2) + 1);
		velocity.z = velocity.z / (pow(velocityLength, 2) + 1);

		mod->SetRotation(0, asin(velocity.x / velocityLength), asin(velocity.y/ velocityLength));
	}

	mod->AddPosition(velocity.x, velocity.y, velocity.z);
	mod->SetScale(swellPercent * (size + length), size * swellPercent, size * swellPercent);
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
					break;
				}		
			
				if (i == Type_InformationFeeder)
				{
					neuralInps.push_back(new InformationFeeder(this, dna, searchStart));
					break; //only one InformationFeeder should be created since they would just override each other and otherwise all "I"'s in the DNA get clustered with new InfFeeder
				}

				if (i == Type_EnergyManager)
				{
					//traits.push_back(new EnergyManager(this, dna, searchStart));
					break;
				}

				if (i == Type_SplittingManager)
				{
					SplittingManager* SM = new SplittingManager(this, dna, searchStart);
					traits.push_back(SM);
					break;
				}
			}
		}
	}
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

string Cell::GetOutputString()
{
	XMFLOAT4 pos = *mod->GetPosition();
	string buffer = "";

	buffer += " Cell ID: " + to_string(ID) + "\n";
	buffer += " Cell Position: " + to_string(pos.x) + "/" + to_string(pos.y) + "/" + to_string(pos.z) + "\n";
	buffer += " Cell Speed: " + to_string(velocity.x) + "/" + to_string(velocity.y) + "/" + to_string(velocity.z) + "\n";
	buffer += " Cell Volume/surface: " + to_string(volume) + "/" + to_string(surfaceArea) + "\n";
	buffer += " Cell size/length: " + to_string(size) + "/" + to_string(length) + "\n";
	buffer += " Cell Swell Percent: "  + to_string(swellPercent) + "\n";
	buffer += " Cell Speed: " + to_string(velocity.x) + "/" + to_string(velocity.y) + "/" + to_string(velocity.z) + "\n";
	buffer += " Cell ATP: " + to_string(ATP) + "\n\n";


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

	buffer += "  Temperature: " + to_string(chemCon->GetTemperature()) + "\n";
	for (int i = 0; i < contains_amount; i++)
	{
		buffer += "  " + writtenSubstances[i] + ": " + to_string(chemCon->GetContains()[i]) + "\n";
	}

	buffer += "\n Chunk (" + to_string(chunk->GetX()) + "/" + to_string(chunk->GetY()) + "/" + to_string(chunk->GetZ()) + ") contains: \n";

	buffer += "  Temperature: " + to_string(chunk->GetChemCon()->GetTemperature()) + "\n";
	for (int i = 0; i < contains_amount; i++)
	{
		buffer += "  " + writtenSubstances[i] + ": " + to_string(chunk->GetChemCon()->GetContains()[i]) + "\n";
	}

	buffer += "\n Traits: \n";
	for (Trait* trt : traits)
	{
		buffer += trt->GetOutputString();
	}

	return buffer;
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
}
