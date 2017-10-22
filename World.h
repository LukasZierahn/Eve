#ifndef __H__World
#define __H__World

#include "include.h"

class Chunk;
class Cell;
class RenderClass;
class ChemicalPush;

class World
{
private:
	int chunkSize;
	int chunkVolume;
	int sizeX, sizeY, sizeZ; //z is depth, this size is given in chunks, the real size is this size * chunksize
	int clock;

	float chemConFlowSpeed = 1;

	RenderClass* render = nullptr;

	vector<Chunk*> chunkVec;
	vector<Cell*> cellVec;

	vector<ChemicalPush*> chemPushVec;

public:
	World(RenderClass* rndCls, int cSize, int s);
	World(RenderClass* rndCls, int cSize, int sX, int sY, int sZ);

	void AddCell(float x, float y, float z);
	Chunk* GetChunk(int x, int y, int z);

	float GetChemConFlowSpeed() { return chemConFlowSpeed; };
	void AddChemPush(ChemicalPush* CP) { chemPushVec.push_back(CP); };

	string GetInfoWindowString();

	void Tick(float t);

	~World();
};

#endif