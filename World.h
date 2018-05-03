#ifndef __H__World
#define __H__World

#include "include.h"
#include "Cell.h"
#include "ChemicalContainer.h"

#define DriftingSpeed 0.00005f

struct CellInformation
{
	int traitCount[Type_Absolute_Amount];

	float membranePassive[contains_amount];

	float membraneAktiveChunkToCell[contains_amount];
	float membraneAktiveCellToChunk[contains_amount];

	int neuralNetworkNodes[3];
	int neuralNetworkSources[Neural_InpNode_Total_Sources];

	float energyManagerCC = 0.0f;
	float energyManagerOxygenCC = 0.0f;
	int splittingManagerPotenz = 0.0f;

};

const static string traitNames[Type_Absolute_Amount] = { "Flagellum", "Membran", "Information Feeder", "Energy Manager", "Splitting Manager", "Oxygen Energy Manager" };

class Chunk;
class Model;
class RenderClass;

class World
{
private:
	int chunkSize; //this is given in micrometers, um = 10^-6 m
	int chunkVolume; //this is given in um^3
	int chunkSurfaceArea; //this is given in um^2
	int sizeX, sizeY, sizeZ; //z is depth, this size is given in chunks, the real size is this size * chunksize
	unsigned long clock = 0.0f;

	int currentTry = 0;
	int currentSafe = 1;
	int maxCells = 0;
	int cellsCreated = 0;
	int oldCellsCreated = 0;
	int cellsDied = 0;
	int oldCellsDied = 0;
	int deathBySwelling = 0;
	int oldDeathBySwelling = 0;
	int deathByATPLack = 0;
	int deathByATPLackAndSplitting = 0;
	int oldDeathByATPLack = 0;
	int oldDeathByATPLackAndSplitting = 0;
	int faultyCells = 0;
	string currentTestRun = "Normal I";
	ofstream* output = nullptr;

	float chemConFlowSpeed = 1;
	unsigned long IDPosition = 1; //this is the next free ID

	RenderClass* render = nullptr;

	Chunk** chunkArray;
	int chunkArraySize;

	//chunkDiffusionArray is a subset of chunkArray and contains every chunk if x + y + z of that chunk is even
	Chunk** chunkDiffusionArray;
	int chunkDiffusionArraySize;

	vector<Cell*> cellVec;

public:
	World(RenderClass* rndCls, int cSize, int s) : World(rndCls, cSize, s, s, s) {};
	World(RenderClass* rndCls, int cSize, int sX, int sY, int sZ);

	void AddCell(Cell* c) { cellVec.push_back(c); cellVec.shrink_to_fit(); maxCells = max(maxCells, cellVec.size()); };
	unsigned long GetNextID() { return IDPosition++; }

	Chunk* GetChunk(int x, int y, int z);

	float GetChemConFlowSpeed() { return chemConFlowSpeed; };
	int GetChunkSize() { return chunkSize; };
	RenderClass* GetRenderClass() { return render; };

	int GetSizeX() { return sizeX; };
	int GetSizeY() { return sizeY; };
	int GetSizeZ() { return sizeZ; };

	vector<Cell*>* GetCellVec() { cellVec.shrink_to_fit(); return &cellVec; }
	Cell* GetCell(int ind) { return cellVec[ind]; }
	void RemoveCell(unsigned long id)
	{
		for (int i = 0; i < cellVec.size(); i++)
		{
			if (cellVec[i]->GetID() == id)
			{
				cellVec.erase(cellVec.begin() + i);
				cellsDied++;
			}
		}
	}

	void GetChunkPos(__in float x, __in float y, __in float z, __out int *outX, __out int *outY, __out int *outZ);
	void GetChunkPos(__in XMFLOAT4 inp, __out int *outX, __out int *outY, __out int *outZ) { GetChunkPos(inp.x, inp.y, inp.z, outX, outY, outZ); }

	void KeepPointInBounds(__inout float* x, __inout float* y, __inout float* z);
	bool CheckIfModelsIntersect(Model* mod1, Model* mod2);

	void IncreaseCellsCreated() { cellsCreated++; }
	void IncreaseDeathByATPLack(bool splitting) 
	{ 
		deathByATPLack++;
		if (splitting)
		{
			deathByATPLackAndSplitting++;
		}
	}
	void IncreaseDeathBySwelling() { deathBySwelling++; }
	void IncreaseFaultyCells() { faultyCells++; }

	void OpenOutputAndIncreaseTry();
	void WriteLog();

	float GetTotalContainingsFromID(int ID);

	string GetCurrentTestRunName() { return currentTestRun; }

	void Reset();

	string GetInfoWindowString();

	void Tick(float t);

	~World();
};

#endif