#include "Chunk.h"
#include "ChemicalContainer.h"
#include "World.h"

Chunk::Chunk(int x, int y, int z, World* w, float vol, float surArea) : world(w)
{
	this->x = x;
	this->y = y;
	this->z = z;

	ChemCon = new ChemicalContainer(world, vol, surArea);
}

void Chunk::AquireNeighbours()
{

	//the six directly adjacent Chunks are added first
	if (x != 0)
		ChemCon->AddSurroundingChunk(world->GetChunk(x - 1, y, z)->GetChemCon());
	if (x != world->GetSizeX() - 1)
		ChemCon->AddSurroundingChunk(world->GetChunk(x + 1, y, z)->GetChemCon());
	if (y != 0)
		ChemCon->AddSurroundingChunk(world->GetChunk(x, y - 1, z)->GetChemCon());
	if (y != world->GetSizeY() - 1)
		ChemCon->AddSurroundingChunk(world->GetChunk(x, y + 1, z)->GetChemCon());
	if (z != 0)
		ChemCon->AddSurroundingChunk(world->GetChunk(x, y, z - 1)->GetChemCon());
	if (z != world->GetSizeZ() - 1)
		ChemCon->AddSurroundingChunk(world->GetChunk(x, y, z + 1)->GetChemCon());

	//the eight semi adjacent chunks are next
	if (x != 0 && y != 0)
		ChemCon->AddSurroundingChunk(world->GetChunk(x - 1, y - 1, z)->GetChemCon());
	if (x != 0 && z != 0)
		ChemCon->AddSurroundingChunk(world->GetChunk(x - 1, y, z - 1)->GetChemCon());

	if (x != 0 && y != world->GetSizeY() - 1)
		ChemCon->AddSurroundingChunk(world->GetChunk(x - 1, y + 1, z)->GetChemCon());
	if (x != 0 && z != world->GetSizeZ() - 1)
		ChemCon->AddSurroundingChunk(world->GetChunk(x - 1, y, z + 1)->GetChemCon());

	if (x != world->GetSizeX() - 1 && y != 0)
		ChemCon->AddSurroundingChunk(world->GetChunk(x + 1, y - 1, z)->GetChemCon());
	if (x != world->GetSizeX() - 1 && z != 0)
		ChemCon->AddSurroundingChunk(world->GetChunk(x + 1, y, z - 1)->GetChemCon());

	if (x != world->GetSizeX() - 1 && y != world->GetSizeY() - 1)
		ChemCon->AddSurroundingChunk(world->GetChunk(x + 1, y + 1, z)->GetChemCon());
	if (x != world->GetSizeX() - 1 && z != world->GetSizeZ() - 1)
		ChemCon->AddSurroundingChunk(world->GetChunk(x + 1, y, z + 1)->GetChemCon());

	//the eight corners are last
	if (x != 0 && y != 0 && z != 0)
		ChemCon->AddSurroundingChunk(world->GetChunk(x - 1, y - 1, z - 1)->GetChemCon());
	if (x != 0 && y != 0 && z != world->GetSizeZ() - 1)
		ChemCon->AddSurroundingChunk(world->GetChunk(x - 1, y - 1, z + 1)->GetChemCon());

	if (x != 0 && y != world->GetSizeY() - 1 && z != 0)
		ChemCon->AddSurroundingChunk(world->GetChunk(x - 1, y + 1, z - 1)->GetChemCon());
	if (x != 0 && y != world->GetSizeY() - 1 && z != world->GetSizeZ() - 1)
		ChemCon->AddSurroundingChunk(world->GetChunk(x - 1, y + 1, z + 1)->GetChemCon());

	if (x != world->GetSizeX() - 1 && y != 0 && z != 0)
		ChemCon->AddSurroundingChunk(world->GetChunk(x + 1, y - 1, z - 1)->GetChemCon());
	if (x != world->GetSizeX() - 1 && y != 0 && z != world->GetSizeZ() - 1)
		ChemCon->AddSurroundingChunk(world->GetChunk(x + 1, y - 1, z + 1)->GetChemCon());

	if (x != world->GetSizeX() - 1 && y != world->GetSizeY() - 1 && z != 0)
		ChemCon->AddSurroundingChunk(world->GetChunk(x + 1, y + 1, z - 1)->GetChemCon());
	if (x != world->GetSizeX() - 1 && y != world->GetSizeY() - 1 && z != world->GetSizeZ() - 1)
		ChemCon->AddSurroundingChunk(world->GetChunk(x + 1, y + 1, z + 1)->GetChemCon());
}


Chunk::~Chunk()
{
	delete ChemCon;
}
