#ifndef __H__World
#define __H__World

#include "include.h"

class Chunk;
class Cell;
class RenderClass;

class World
{
private:
	int chunkSize; //this is given in micrometers, um = 10^-6 m
	int chunkVolume; //this is given in um^3
	int sizeX, sizeY, sizeZ; //z is depth, this size is given in chunks, the real size is this size * chunksize
	int clock;

	float chemConFlowSpeed = 1;

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

	void AddCell(Cell* c) { cellVec.push_back(c); };

	Chunk* GetChunk(int x, int y, int z);

	float GetChemConFlowSpeed() { return chemConFlowSpeed; };
	int GetChunkSize() { return chunkSize; };

	int GetSizeX() { return sizeX; };
	int GetSizeY() { return sizeY; };
	int GetSizeZ() { return sizeZ; };

	vector<Cell*>* GetCellVec() { return &cellVec; }

	void GetChunkPos(__in float x, __in float y, __in float z, __out int *outX, __out int *outY, __out int *outZ);
	void GetChunkPos(__in XMFLOAT4 inp, __out int *outX, __out int *outY, __out int *outZ) { GetChunkPos(inp.x, inp.y, inp.z, outX, outY, outZ); }
	void KeepPointInBounds(__inout float* x, __inout float* y, __inout float* z);

	string GetInfoWindowString();

	void Tick(float t);

	~World();
};

#endif