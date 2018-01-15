#include "World.h"
#include "Chunk.h"
#include "RenderClass.h"
#include "Cell.h"
#include "ChemicalContainer.h"
#include "Camera.h"
#include "Model.h"

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
			}
		}
	}

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			for (int z = 0; z < sizeZ; z++)
			{
				if ((z + x + y) % 2 == 0)
				{
					chunkArray[(x * sizeZ * sizeY) + (y * sizeZ) + z]->AquireNeighbours();
					chunkDiffusionArray[chunkDiffusionArrayCounter] = chunkArray[(x * sizeZ * sizeY) + (y * sizeZ) + z];
					chunkDiffusionArrayCounter++;
				}
			}
		}
	}
}

Chunk* World::GetChunk(int posX, int posY, int posZ)
{
	return chunkArray[(posX * sizeZ * sizeY) + (posY * sizeZ) + posZ];
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

bool World::CheckIfModelsIntersect(Model * mod1, Model * mod2)
{
	XMFLOAT4* mod1Pos = mod1->GetPosition();
	XMFLOAT4 mod1Box = mod1->GetBoundingBox();

	XMFLOAT4* mod2Pos = mod2->GetPosition();
	XMFLOAT4 mod2Box = mod2->GetBoundingBox();

	if (abs(mod1Box.x - mod2Box.x) > abs(mod1Pos->x - mod2Pos->x))
		return false;
	if (abs(mod1Box.y - mod2Box.y) > abs(mod1Pos->y - mod2Pos->y))
		return false;
	if (abs(mod1Box.z - mod2Box.z) > abs(mod1Pos->z - mod2Pos->z))
		return false;

	/*for (int i = 0; i < 8; i++)
	{
		if (mod1Pos->x + (mod1Box.x * ((i / 4) * 2 - 1))  < mod2Pos->x + mod2Box.x && mod1Pos->x + mod1Box.x * ((i / 4) * 2 - 1) > mod2Pos->x - mod2Box.x)
		{
			if (mod1Pos->y - mod1Box.y < mod2Pos->y + mod2Box.y && mod1Pos->y + mod1Box.y > mod2Pos->y - mod2Box.y)
			{
				if (mod1Pos->z - mod1Box.z < mod2Pos->z + mod2Box.z && mod1Pos->z + mod1Box.z > mod2Pos->z - mod2Box.z)
				{
					return true;
				}
			}
		}
	}*/
	return true;
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


	for (int i = 0; i < cellVec.size(); i++)
	{
		for (int j = i + 1; j < cellVec.size(); j++)
		{
			if (CheckIfModelsIntersect(cellVec[i]->GetModel(), cellVec[j]->GetModel()))
			{
				XMFLOAT4* iCellPos = cellVec[i]->GetPosition();
				XMFLOAT4* jCellPos = cellVec[j]->GetPosition();

				float distance = sqrt(pow(iCellPos->x - jCellPos->x, 2) + pow(iCellPos->y - jCellPos->y, 2) + pow(iCellPos->z - jCellPos->z, 2));

				float test2 = iCellPos->x - jCellPos->x / distance;
				float test = asin((iCellPos->x - jCellPos->x) / distance);

				cellVec[i]->AddVelocity(asin((iCellPos->x - jCellPos->x) / distance) * DriftingSpeed * t, asin((iCellPos->y - jCellPos->y) / distance) * DriftingSpeed * t, -asin((iCellPos->z - jCellPos->z) / distance) * DriftingSpeed * t);
				cellVec[j]->AddVelocity(-asin((iCellPos->x - jCellPos->x) / distance) * DriftingSpeed * t, -asin((iCellPos->y - jCellPos->y) / distance) * DriftingSpeed * t, asin((iCellPos->z - jCellPos->z) / distance) * DriftingSpeed * t);
			}
		}
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
