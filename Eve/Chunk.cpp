#include "Chunk.h"
#include "ChemicalContainer.h"
#include "World.h"

Chunk::Chunk(int x, int y, int z, World* world, int vol)
{
	this->x = x;
	this->y = y;
	this->z = z;

	ChemCon = new ChemicalContainer(world, vol);
}


Chunk::~Chunk()
{
	delete ChemCon;
}
