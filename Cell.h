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
class Trait;
class NeuralNetworkInput;

#define Type_Flagellum 0
#define Type_Membrane 1
#define Type_InformationFeeder 2
#define Type_EnergyManager 3
#define Type_SplittingManager 4

#define Type_Absolute_Amount 5

class Cell
{
private:
	RenderClass* render = nullptr;
	Model* mod = nullptr;

	World* world = nullptr;
	ChemicalContainer* chemCon = nullptr;
	Chunk* chunk = nullptr; //the chunk the cell is currently in

	DNA* dna = nullptr;
	NeuralNetwork* neuralNet = nullptr;

	float volume;
	float surfaceArea;
	int chunkSize;
	float size = 0.0f;
	float length = 0.0f;

	bool hasMembrane = false;

	float ATP = 0;

	float buildingCost = 0;

	XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };

	vector<NeuralNetworkInput*> neuralInps;
	vector<Trait*> traits;

	const static string dnaCriteria[];
public:
	Cell(RenderClass* rndCls, World* world, DNA* dna = nullptr);
	Cell(RenderClass* rndCls, World* world, DNA* dna, float x, float y, float z);

	void Tick(float t);
	void CheckDNAForTraits();

	World* GetWorld() { return world; }
	bool BuildCell(float buildAmount) { buildingCost -= buildAmount; return buildingCost <= 0; }
	float GetBuildingCost() { return buildingCost; }
	float GetATP() { return ATP; }
	float GetSurfaceArea() { return surfaceArea; }

	Model* GetModel() { return mod; }
	RenderClass* GetRenderClass() { return render; }
	ChemicalContainer* GetChemCon() { return chemCon; }

	XMFLOAT4* GetPosition() { return mod->GetPosition(); }
	float GetPositionX() { return mod->GetPosition()->x; }
	float GetPositionY() { return mod->GetPosition()->y; }
	float GetPositionZ() { return mod->GetPosition()->z; }

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT4* pos) { SetPosition(pos->x, pos->y, pos->z); }
	void AddPosition(float x, float y, float z);

	float GetSpeedX() { return velocity.x; }
	float GetSpeedY() { return velocity.y; }
	float GetSpeedZ() { return velocity.z; }

	void SetVelocity(XMFLOAT3* vel) { velocity = *vel; }
	XMFLOAT3* GetVelocity() { return &velocity; }

	void SetMembraneStatus(bool mem) { hasMembrane = mem; }
	Chunk* GetCurrentChunk() { return chunk; }

	NeuralNetwork* GetNeuralNetwork() { return neuralNet; }


	string GetOutputString();

	~Cell();
};

#endif