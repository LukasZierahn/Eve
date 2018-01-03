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
#include "Flagellum.h"

const static string dnaCriteria[] = { "Fl" };

Cell::Cell(RenderClass* rndCls, World* world)
{
	volume = 10000; //this is not finalised

	velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	mod = new Model(rndCls);
	this->world = world;
	chemCon = new ChemicalContainer(world, volume);

	dna = new DNA();
	dna->GenerateRandomDNA(94 * 4);
	OutputDebugString(dna->GetString().c_str());

	neuralNet = new NeuralNetwork();
	neuralNet->BuildFromDNA(dna);

	chunkSize = world->GetChunkSize();

	rndCls->GetModelLoader()->GetModel("Cell", mod->GetDataPointer());
	rndCls->GetModelLoader()->GetTexture("Cell", mod->GetTexturePointer());

	rndCls->AddModel(mod);

	Flagellum* Fl = new Flagellum(this, dna, 10);
	Trait* trait = new Trait;
	trait->pointer = Fl;
	trait->type = Type_Flagellum;
	traits.push_back(trait);
}

Cell::Cell(RenderClass* rndCls, World* world, float x, float y, float z) : Cell(rndCls, world)
{
	SetPosition(x, y, z);
}

void Cell::Tick(float t)
{
	for (Trait* trt : traits)
	{
		if (trt->type == Type_Flagellum)
		{
			static_cast<Flagellum*> (trt->pointer)->Tick(t);
		}
	}

	//this is drag by the water
	velocity.x -= t * pow(velocity.x, 2) / 10.0f;
	velocity.y -= t * pow(velocity.y, 2) / 10.0f;
	velocity.z -= t * pow(velocity.z, 2) / 10.0f;

	mod->AddPosition(velocity.x, velocity.y, velocity.z);

	float velocitySum = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));
	mod->SetRotation(asin(velocity.x / velocitySum), asin(velocity.y / velocitySum), -asin(velocity.z / velocitySum));

	XMFLOAT4 pos = *mod->GetPosition();

	chunk = world->GetChunk(floor(pos.x / chunkSize), floor(pos.y / chunkSize), floor(pos.z / chunkSize));

	chemCon->DiffuseFromAndTo(chunk->GetChemCon(), t);

	chemCon->ApplyContains();
}

void Cell::CheckDNAForTraits()
{
	for (int i = 0; i < dnaCriteria->size(); i++)
	{
		int searchStart = 0;
		while (searchStart != string::npos)
		{
			searchStart = dna->GetString().find(dnaCriteria[i], searchStart);
			if (searchStart != string::npos)
			{
				Trait* trait = new Trait;

				if (i == Type_Flagellum)
				{
					Flagellum* Fl = new Flagellum(this, dna, searchStart);
					trait->pointer = Fl;
					trait->type = Type_Flagellum;
				}

				traits.push_back(trait);
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

	buffer += " Cell Position: " + to_string(pos.x) + "/" + to_string(pos.y) + "/" + to_string(pos.z) + "\n";
	buffer += " Cell Speed: " + to_string(velocity.x) + "/" + to_string(velocity.y) + "/" + to_string(velocity.z) + "\n\n";

	buffer += " InputNodes: ";
	for (int i = 0; i < neuralNet->GetInputLayerCount(); i++)
	{
		if (i % 3 == 2)
		{
			buffer += "\n       ";
		}
		buffer += to_string(neuralNet->GetInputNode(i)) + " ";
	}
	buffer += "\n";

	buffer += " hiddenLayerCount: " + to_string(neuralNet->GetHiddenLayerCount()) + "\n";

	buffer += " OutputNodes: ";
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

	buffer += " Temperature: " + to_string(chemCon->GetTemperature()) + "\n";
	for (int i = 0; i < contains_amount; i++)
	{
		buffer += "  " + writtenSubstances[i] + ": " + to_string(chemCon->GetContains()[i]) + "\n";
	}

	buffer += "\n Chunk (" + to_string(chunk->GetX()) + "/" + to_string(chunk->GetY()) + "/" + to_string(chunk->GetZ()) + ") contains: \n";

	buffer += " Temperature: " + to_string(chunk->GetChemCon()->GetTemperature()) + "\n";
	for (int i = 0; i < contains_amount; i++)
	{
		buffer += "  " + writtenSubstances[i] + ": " + to_string(chunk->GetChemCon()->GetContains()[i]) + "\n";
	}
	return buffer;
}

Cell::~Cell()
{
	for (Trait* t : traits)
	{
		delete t->pointer;
		delete t;
	}

	delete chemCon, mod, dna, neuralNet;
}
