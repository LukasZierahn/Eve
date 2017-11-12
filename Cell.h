#ifndef __H__Cell
#define __H__Cell

#include "Model.h"

class RenderClass;
class ChemicalContainer;
class World;
class Chunk;

class Cell
{
private:
	Model* mod = nullptr;

	World* world = nullptr;
	ChemicalContainer* chemCon = nullptr;
	Chunk* chunk = nullptr; //the chunk the cell is currently in

	int volume;
	int chunkSize;

public:
	Cell(RenderClass* rndCls, World* world);
	Cell(RenderClass* rndCls, World* world, float x, float y, float z);

	void Tick(float t);

	Model* GetModel() { return mod; }
	ChemicalContainer* GetChemCon() { return chemCon; }

	float GetPositionX() { return mod->GetPosition()->x; }
	float GetPositionY() { return mod->GetPosition()->y; }
	float GetPositionZ() { return mod->GetPosition()->z; }

	Chunk* GetCurrentChunk() { return chunk; }

	void SetPosition(float x, float y, float z);
	void AddPosition(float x, float y, float z);

	~Cell();
};

#endif