#include "World.h"
#include "Chunk.h"
#include "RenderClass.h"
#include "Cell.h"
#include "ChemicalContainer.h"


World::World(RenderClass* rndCls, int cSize, int s) : World(rndCls, cSize, s, s, s)
{

}

World::World(RenderClass* rndCls, int cSize, int sX, int sY, int sZ)
{
	render = rndCls;
	chunkSize = cSize;
	chunkVolume = pow(chunkSize, 3);
	sizeX = sX;
	sizeY = sY;
	sizeZ = sZ;

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			for (int z = 0; z < sizeZ; z++)
			{
				chunkVec.push_back(new Chunk(x, y, z, this, chunkVolume));
			}
		}
	}

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			for (int z = 0; z < sizeZ; z++)
			{
				chunkVec[(x * sizeZ * sizeY) + (y * sizeZ) + z]->AquireNeighbours();
			}
		}
	}

}

Chunk* World::GetChunk(int x, int y, int z)
{
	return chunkVec[(x * sizeZ * sizeY) + (y * sizeZ) + z];
}

string World::GetInfoWindowString()
{
	string buffer = "";

	buffer += " Chunk Size: " + to_string(chunkSize) + "\n";
	buffer += " Chunk Volume : " + to_string(chunkVolume) + "\n";
	buffer += " Total Chunks: " + to_string(chunkVec.size()) + "  (" + to_string(sizeX) + " / " + to_string(sizeY) + " / " + to_string(sizeZ) + ")\n";
	buffer += " Cells Alive: " + to_string(cellVec.size()) + "\n";
	buffer += " Containings of 0th Chunk: \n";
	/*for (map<string, float> ::iterator it = chunkVec.at(0)->GetChemCon()->GetContains()->begin(); it != chunkVec.at(0)->GetChemCon()->GetContains()->end(); ++it)
	{
		buffer += "   " + it->first + ": " + to_string(it->second) + "\n";
	}

	buffer += " Containings of 1th Chunk: \n";
	for (map<string, float> ::iterator it = chunkVec.at(100)->GetChemCon()->GetContains()->begin(); it != chunkVec.at(100)->GetChemCon()->GetContains()->end(); ++it)
	{
		buffer += "   " + it->first + ": " + to_string(it->second) + "\n";
	}*/

	return buffer;
}

void World::Tick(float t)
{
	for (Chunk* c : chunkVec)
	{
		c->GetChemCon()->DiffuseToNeighbouringChunks(t);
	}

	for (Chunk* c : chunkVec)
	{
		c->GetChemCon()->ApplyContains();
	}
}

void World::AddCell(float x, float y, float z)
{
	cellVec.push_back(new Cell(render, x, y, z));
}

World::~World()
{
	delete render;
	for (Cell* c : cellVec)
	{
		delete c;
	}
}
