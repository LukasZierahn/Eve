#include "World.h"
#include "Chunk.h"
#include "RenderClass.h"
#include "Cell.h"
#include "ChemicalContainer.h"
#include "Camera.h"

World::World(RenderClass* rndCls, int cSize, int sX, int sY, int sZ)
{
	render = rndCls;
	chunkSize = cSize;
	chunkVolume = pow(chunkSize, 3);
	chunkSurfaceArea = 6 * pow(chunkSize, 2);
	sizeX = sX;
	sizeY = sY;
	sizeZ = sZ;
	chunkArraySize = sizeX * sizeY * sizeZ;
	chunkArray = new Chunk* [chunkArraySize];
	chunkDiffusionArraySize = floor(chunkArraySize / 2);
	chunkDiffusionArray = new Chunk* [chunkDiffusionArraySize];

	int chunkDiffusionArrayCounter = 0;

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			for (int z = 0; z < sizeZ; z++)
			{
				chunkArray[(x * sizeZ * sizeY) + (y * sizeZ) + z] = new Chunk(x, y, z, this, chunkVolume, chunkSurfaceArea);
				if ((z + x + y) % 2 == 0)
				{
					chunkDiffusionArray[chunkDiffusionArrayCounter] = chunkArray[z * y * x];
					chunkDiffusionArrayCounter++;
				}
			}
		}
	}

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			for (int z = 0; z < sizeZ; z++)
			{
				chunkArray[(x * sizeZ * sizeY) + (y * sizeZ) + z]->AquireNeighbours();
			}
		}
	}

}

Chunk* World::GetChunk(int x, int y, int z)
{
	return chunkArray[(x * sizeZ * sizeY) + (y * sizeZ) + z];
}

void World::GetChunkPos(float x, float y, float z, int* outX, int* outY, int* outZ)
{
	*outX = floor(x / chunkSize);
	if (*outX < 0) *outX = 0;
	if (*outX >= sizeX) *outX = sizeX - 1;

	*outY = floor(y / chunkSize);
	if (*outY < 0) *outY = 0;
	if (*outY >= sizeY) *outY = sizeY - 1;

	*outZ = floor(z / chunkSize);
	if (*outZ < 0) *outZ = 0;
	if (*outZ >= sizeZ) *outZ = sizeZ - 1;
}

void World::KeepPointInBounds(float* x, float* y, float* z)
{
	while (*x < 0) { *x = chunkSize * (sizeX - 1) + *x; }
	while (*y < 0) { *y = chunkSize * (sizeX - 1) + *y; }
	while (*z < 0) { *z = chunkSize * (sizeX - 1) + *z; }

	while (*x > chunkSize * (sizeX)) { *x = *x - chunkSize * (sizeX); }
	while (*y > chunkSize * (sizeY)) { *y = *y - chunkSize * (sizeY); }
	while (*z > chunkSize * (sizeZ)) { *z = *z - chunkSize * (sizeZ); }
}

string World::GetInfoWindowString()
{
	string buffer = "";

	buffer += " Chunk Size: " + to_string(chunkSize) + "\n";
	buffer += " Chunk Volume : " + to_string(chunkVolume) + "\n";
	buffer += " Total Chunks: " + to_string(chunkArraySize) + "  (" + to_string(sizeX) + " / " + to_string(sizeY) + " / " + to_string(sizeZ) + ")\n";
	buffer += " Cells Alive: " + to_string(cellVec.size()) + "\n";

	double total = 0;

	for (int i = 0; i < chunkArraySize; i++)
	{
		total += chunkArray[i]->GetChemCon()->GetContains()[0];
	}

	for (Cell* c : cellVec)
	{
		total += c->GetChemCon()->GetContains()[0];
	}

	buffer += " Total Na: " + to_string(total) + "\n";

	XMFLOAT4 camPos = *render->GetCamera()->GetPosition();
	int chunkPosX, chunkPosY, chunkPosZ;
	GetChunkPos(camPos, &chunkPosX, &chunkPosY, &chunkPosZ);

	buffer += " Chunk Temperature: " + to_string(GetChunk(chunkPosX, chunkPosY, chunkPosZ)->GetChemCon()->GetTemperature()) + "° \n";

	buffer += " Currently in Chunk: " + to_string(chunkPosX) + "/" + to_string(chunkPosY) + "/" + to_string(chunkPosZ) + "\n";

	for (int i = 0; i < contains_amount; i++)
	{
		buffer += "  " + writtenSubstances[i] + ": " + to_string(GetChunk(chunkPosX, chunkPosY, chunkPosZ)->GetChemCon()->GetContains()[i]) + "\n";
	}

	return buffer;
}

void World::Tick(float inpT)
{
	float t = min(inpT, 100.0f);

	for (int i = 0; i < chunkDiffusionArraySize; i++)
	{
		chunkDiffusionArray[i]->GetChemCon()->DiffuseToNeighbouringChunks(t);
	}

	for (int i = 0; i < cellVec.size(); i++)
	{
		cellVec[i]->Tick(t);
	}

	for (int i = 0; i < chunkArraySize; i++)
	{
		chunkArray[i]->GetChemCon()->ApplyContains();
	}
}

World::~World()
{
	delete render;
	delete[] chunkArray;
	delete[] chunkDiffusionArray;
	
	for (Cell* c : cellVec)
	{
		delete c;
	}
}
