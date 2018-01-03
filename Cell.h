#ifndef __H__Cell
#define __H__Cell

#include "Model.h"
#include "include.h"

class RenderClass;
class ChemicalContainer;
class World;
class Chunk;
class DNA;
class NeuralNetwork;

#define Type_Flagellum 1

//struct Trait
{
	void* pointer = nullptr;
	int type = 0; //why cant i just store a reference to the type of class i am using?
};


class Cell
{
private:
	Model* mod = nullptr;

	World* world = nullptr;
	ChemicalContainer* chemCon = nullptr;
	Chunk* chunk = nullptr; //the chunk the cell is currently in

	DNA* dna = nullptr;
	NeuralNetwork* neuralNet = nullptr;

	int volume;
	int chunkSize;

	int ATP = 0;

	XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };

	vector<Trait*> traits;

	const static string dnaCriteria[];
public:
	Cell(RenderClass* rndCls, World* world);
	Cell(RenderClass* rndCls, World* world, float x, float y, float z);

	void Tick(float t);
	void CheckDNAForTraits();

	Model* GetModel() { return mod; }
	ChemicalContainer* GetChemCon() { return chemCon; }

	float GetPositionX() { return mod->GetPosition()->x; }
	float GetPositionY() { return mod->GetPosition()->y; }
	float GetPositionZ() { return mod->GetPosition()->z; }

	float GetSpeedX() { return velocity.x; }
	float GetSpeedY() { return velocity.y; }
	float GetSpeedZ() { return velocity.z; }

	void SetVelocity(XMFLOAT3* vel) { velocity = *vel; }
	XMFLOAT3* GetVelocity() { return &velocity; }

	Chunk* GetCurrentChunk() { return chunk; }

	NeuralNetwork* GetNeuralNetwork() { return neuralNet; }

	void SetPosition(float x, float y, float z);
	void AddPosition(float x, float y, float z);

	string GetOutputString();

	~Cell();
};

#endif