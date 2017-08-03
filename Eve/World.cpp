#include "World.h"
#include "Chunk.h"
#include "RenderClass.h"
#include "Cell.h"


int World::WriteString(HDC hDC, string toWrite, int pos)
{

	TextOut(hDC, 10, pos * 20, toWrite.c_str(), toWrite.length());
	return ++pos;
}

World::World(RenderClass* rndCls, HWND hW, HINSTANCE hIns, int cSize, int s) : World(rndCls, hW, hIns, cSize, s, s, s)
{

}

World::World(RenderClass* rndCls, HWND hW, HINSTANCE hIns, int cSize, int sX, int sY, int sZ)
{
	render = rndCls;
	hWnd = hW;
	hInstance = hIns;
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
				chunkVec.push_back(new Chunk(x, y, z));
			}
		}
	}

	RECT rect;
	GetWindowRect(hWnd, &rect);
	textLabel = CreateWindow("STATIC", "not initilized", WS_VISIBLE | WS_CHILD | SS_LEFT, 0, 0, rect.right, 100, hWnd, NULL, hInstance, NULL);
	WriteInfoData();
}

Chunk* World::GetChunk(int x, int y, int z)
{
	return chunkVec[(x * sizeZ * sizeY) + (y * sizeZ) + z];
}

void World::WriteInfoData()
{
	string buffer = "";

	buffer += " Chunk Size: " + to_string(chunkSize) + "\n";
	buffer += " Chunk Volume : " + to_string(chunkVolume) + "\n";
	buffer += " Total Chunks: " + to_string(chunkVec.size()) + "  (" + to_string(sizeX) + " / " + to_string(sizeY) + " / " + to_string(sizeZ) + ")\n";
	buffer += " Cells Alive: " + to_string(cellVec.size()) + "\n";

	SetWindowText(textLabel, TEXT(buffer.c_str()));
}

void World::AddCell(float x, float y, float z)
{
	cellVec.push_back(new Cell(render, x, y, z));
	WriteInfoData();
}

World::~World()
{
	for (Cell* c : cellVec)
	{
		delete c;
	}
}
