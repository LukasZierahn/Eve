#include "World.h"
#include "Chunk.h"
#include "RenderClass.h"
#include "Cell.h"
#include "ChemicalContainer.h"
#include "Camera.h"
#include "Model.h"
#include "DNA.h"

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
	if (isinf(*x) || isinf(*y) || isinf(*z))
	{
		return;
	}

	while (*x < 0) { *x = chunkSize * (sizeX - 1) + *x; }
	while (*y < 0) { *y = 0; }
	while (*z < 0) { *z = chunkSize * (sizeX - 1) + *z; }

	while (*x >= chunkSize * (sizeX)) { *x = *x - chunkSize * (sizeX); }
	if (*y >= chunkSize * (sizeY)) { *y = chunkSize * (sizeY); }
	while (*z >= chunkSize * (sizeZ)) { *z = *z - chunkSize * (sizeZ); }
}

bool World::CheckIfModelsIntersect(Model * mod1, Model * mod2)
{
	XMFLOAT4* mod1Pos = mod1->GetPosition();
	XMFLOAT4 mod1Box = mod1->GetBoundingBox();

	XMFLOAT4* mod2Pos = mod2->GetPosition();
	XMFLOAT4 mod2Box = mod2->GetBoundingBox();

	float distance = pow(mod1Pos->x - mod2Pos->x, 2) + pow(mod1Pos->y - mod2Pos->y, 2) + pow(mod1Pos->z - mod2Pos->z, 2);
	float boundingBoxRange = pow(mod1Box.x + mod2Box.x, 2) + pow(mod1Box.y + mod2Box.y, 2) + pow(mod1Box.z + mod2Box.z, 2) * 2 / 3;

	if (distance > boundingBoxRange)
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

void World::OpenOutputAndIncreaseTry()
{
	currentTry++;
	currentSafe = 1;
	maxCells = 0;
	if (output != nullptr)
	{
		output->close();
		delete output;
		output = nullptr;
	}
	output = new ofstream();
	output->open(currentTestRun + "\\" + (currentTestRun + " - " + to_string(currentTry) + ".csv"));
	*output << "Minutes, Seconds, Cell Alive, Maximum Cell Count, Cells Died, Cells Died by Swelling, Cells Died by Lack of ATP, Cells Died by Lack of ATP that were Splitting, Cells Died in the last 30 Seconds, Cells Died in the last 30 Seconds (Swelling), Cells Died in the last 30 Seconds (ATP), Cells Died in the last 30 Seconds (ATP & Splitting), Cells Created, Cells Created in the last 30 Seconds";
	*output << ", Average Cell Lifetime(in seconds), Cell Lifetime Standard Deviation, Average Cell Size, Cell Size Standard Deviation, Average Cell Length, Cell Length Standard Deviation, Flagellum Absolute, Flagellum Percent, Membrane Absolute, Membrane Percent, Energy Manager Absolute, Energy Manager percent, Splitting Manager Absolute, Splitting Manager Percent";
	*output << ", Total Food, Total Poison\n";
	output->flush();
}

void World::WriteLog()
{
	*output << (to_string(clock / 60000) + "," + to_string((clock / 1000) % 60));
	*output << "," + to_string(cellVec.size()) + "," + to_string(maxCells);
	*output << "," + to_string(cellsDied) + "," + to_string(deathBySwelling) + "," + to_string(deathByATPLack) + "," + to_string(deathByATPLackAndSplitting);
	*output << "," + to_string(cellsDied - oldCellsDied) + "," + to_string(deathBySwelling - oldDeathBySwelling) + "," + to_string(deathByATPLack - oldDeathByATPLack) + "," + to_string(deathByATPLackAndSplitting - oldDeathByATPLackAndSplitting);
	*output << "," + to_string(cellsCreated) + "," + to_string(cellsCreated - oldCellsCreated);

	oldDeathByATPLack = deathByATPLack;
	oldDeathByATPLackAndSplitting = deathByATPLackAndSplitting;
	oldDeathBySwelling = deathBySwelling;
	oldCellsDied = cellsDied;
	oldCellsCreated = cellsCreated;

	float averageCellSize = 0;
	float averageCellLength = 0;
	float averageCellAliveTime = 0;

	for (Cell* c : cellVec)
	{
		averageCellSize += c->GetSize();
		averageCellLength += c->GetLength();
		averageCellAliveTime += c->GetTimeAlive() / 1000;
	}

	averageCellSize /= cellVec.size();
	averageCellLength /= cellVec.size();
	averageCellAliveTime /= cellVec.size();

	float standardDeviationCellSize = 0;
	float standardDeviationCellLength = 0;
	float standardDeviationAliveTime = 0;

	for (Cell* c : cellVec)
	{
		standardDeviationCellSize += pow(c->GetSize() - averageCellSize, 2);
		standardDeviationCellLength += pow(c->GetLength() - averageCellLength, 2);
		standardDeviationAliveTime += pow(c->GetTimeAlive() / 1000 - averageCellAliveTime, 2);
	}

	standardDeviationCellSize = sqrt(standardDeviationCellSize / cellVec.size()); //we devide by N instead of N - 1 as we are looking at the whole population
	standardDeviationCellLength = sqrt(standardDeviationCellLength / cellVec.size());
	standardDeviationAliveTime = sqrt(standardDeviationAliveTime / cellVec.size());

	*output << "," + to_string(averageCellAliveTime) + "," + to_string(standardDeviationAliveTime) + "," + to_string(averageCellSize) + "," + to_string(standardDeviationCellSize) + "," + to_string(averageCellLength) + "," + to_string(standardDeviationCellLength);

	int flag = 0;
	int mem = 0;
	int ene = 0;
	int split = 0;

	cellVec.shrink_to_fit();

	for (Cell* c : cellVec)
	{
		c->AddCountForOutput(&flag, &mem, &ene, &split);
	}

	*output << "," + to_string(flag) + "," + to_string(flag * 1.0f / cellVec.size());
	*output << "," + to_string(mem) + "," + to_string(mem * 1.0f / cellVec.size());
	*output << "," + to_string(ene) + "," + to_string(ene * 1.0f / cellVec.size());
	*output << "," + to_string(split) + "," + to_string(split * 1.0f / cellVec.size());

	*output << "," + to_string(GetTotalContainingsFromID(FOOD_CHEMCON_ID)) + "," + to_string(GetTotalContainingsFromID(POISON_CHEMCON_ID));

	*output << "\n";

	output->flush();

	//there have been single cells (approximitly 1:10.000) that have infinte amounts of ATP and thus cant die and replicate indefenitely and that grow exponentially
	//i am not sure what causes these 'zombie' cells but sever time pressure has lead to me to this, admittetly hacky solution.
	//the influence on the simulation itself is negletable as the amount of zombie cells is very small
	for (Cell* c : cellVec)
	{
		if (isinf(c->GetATP()))
		{
			//bad cells get killed
			c->SetATP(0);
		}
	}
}

float World::GetTotalContainingsFromID(int ID)
{

	float total = 0.0f;
	for (int i = 0; i < chunkArraySize; i++)
	{
		total += chunkArray[i]->GetChemCon()->GetContains(ID);
	}

	return total;
}

void World::Reset()
{
	clock = 0;

	deathByATPLack = 0;
	deathByATPLackAndSplitting = 0;
	deathBySwelling = 0;
	oldDeathByATPLack = 0;
	oldDeathByATPLackAndSplitting = 0;
	oldDeathBySwelling = 0;

	cellsDied = 0;
	cellsCreated = 0;
	oldCellsDied = 0;
	oldCellsCreated = 0;

	for (int i = 0; i < chunkArraySize; i++)
	{
		chunkArray[i]->GetChemCon()->SetSubstanceInContains(FOOD_CHEMCON_ID, FOOD_NORMALVALUE);
		chunkArray[i]->GetChemCon()->SetSubstanceInContains(POISON_CHEMCON_ID, 0.0f);
	}
}

string World::GetInfoWindowString()
{
	string buffer = "";

	buffer += " Current Try: " + to_string(currentTry) + "\n";
	buffer += " Chunk Size: " + to_string(chunkSize) + "\n";
	buffer += " Time: " + to_string(clock / 60000) + ":" + to_string((clock / 1000) % 60) + "\n";
	buffer += " Chunk Volume : " + to_string(chunkVolume) + "\n";
	buffer += " Total Chunks: " + to_string(chunkArraySize) + "  (" + to_string(sizeX) + " / " + to_string(sizeY) + " / " + to_string(sizeZ) + ")\n";

	buffer += " Total Food: " + to_string(GetTotalContainingsFromID(FOOD_CHEMCON_ID)) + "\n";

	buffer += " Cells Alive: " + to_string(cellVec.size()) + "/" + to_string(maxCells) + "\n";
	buffer += " Cells Died (ATP,Swelling): " + to_string(deathByATPLack) + "/" + to_string(deathBySwelling) + "\n";

	XMFLOAT4 camPos = *render->GetCamera()->GetPosition();
	int chunkPosX, chunkPosY, chunkPosZ;
	GetChunkPos(camPos, &chunkPosX, &chunkPosY, &chunkPosZ);

	//buffer += " Chunk Temperature: " + to_string(GetChunk(chunkPosX, chunkPosY, chunkPosZ)->GetChemCon()->GetTemperature()) + "° \n";

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
	clock += t;

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

				int x = 1;
				int y = 1;
				int z = 1;

				if (iCellPos->x - jCellPos->x < 0)
					x = -1;
				if (iCellPos->y - jCellPos->y < 0)
					y = -1;
				if (iCellPos->z - jCellPos->z < 0)
					z = -1;

				float distance = sqrt(pow(iCellPos->x - jCellPos->x, 2) + pow(iCellPos->y - jCellPos->y, 2) + pow(iCellPos->z - jCellPos->z, 2));

				cellVec[i]->AddVelocity(asin((iCellPos->x - jCellPos->x) / (distance + 1)) * DriftingSpeed * t * (1 / (distance + 1)), asin((iCellPos->y - jCellPos->y) / (distance + 1)) * DriftingSpeed * t * (1 / (distance + 1)), -asin((iCellPos->z - jCellPos->z) / (distance + 1)) * DriftingSpeed * t * (1 / (distance + 1)));
				cellVec[j]->AddVelocity(-asin((iCellPos->x - jCellPos->x) / (distance + 1)) * DriftingSpeed * t * (1 / (distance + 1)), -asin((iCellPos->y - jCellPos->y) / (distance + 1)) * DriftingSpeed * t * (1 / (distance + 1)), asin((iCellPos->z - jCellPos->z) / (distance + 1)) * DriftingSpeed * t * (1 / (distance + 1)));
			}
		}
	}

	for (int i = 0; i < chunkArraySize; i++)
	{
		chunkArray[i]->GetChemCon()->ContainsNormalisation(t);
		chunkArray[i]->GetChemCon()->ApplyContains();
	}
}

World::~World()
{
	delete render;
	delete[] chunkArray;
	delete[] chunkDiffusionArray;
	if (output)
	{
		output->close();
		delete output;
	}

	for (Cell* c : cellVec)
	{
		delete c;
	}
}
