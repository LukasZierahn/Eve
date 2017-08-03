#ifndef __H__World
#define __H__World

#include "include.h"

class Chunk;
class Cell;
class RenderClass;

class World
{
private:
	int chunkSize;
	int chunkVolume;
	int sizeX, sizeY, sizeZ; //z is depth, this size is given in chunks, the real size is this size * chunksize
	int clock;

	HWND hWnd;
	HWND textLabel;
	HINSTANCE hInstance;

	RenderClass* render = nullptr;

	vector<Chunk*> chunkVec;
	vector<Cell*> cellVec;

	int WriteString(HDC hDC, string toWrite, int pos);

public:
	World(RenderClass* rndCls, HWND hW, HINSTANCE hInstance, int cSize, int s);
	World(RenderClass* rndCls, HWND hW, HINSTANCE hInstance, int cSize, int sX, int sY, int sZ);

	void AddCell(float x, float y, float z);

	Chunk* GetChunk(int x, int y, int z);

	void WriteInfoData();

	~World();
};

#endif