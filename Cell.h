#ifndef __H__Cell
#define __H__Cell

#include "Model.h"
#include "include.h"
#include "Trait.h"

class RenderClass;
class ChemicalContainer;
class World;
class Chunk;
class DNA;
class NeuralNetwork;
class NeuralNetworkInput;
class SplittingManager;

struct CellInformation;

#define Type_Flagellum 0
#define Type_Membrane 1
#define Type_InformationFeeder 2
#define Type_EnergyManager 3
#define Type_SplittingManager 4
#define Type_EnergyManagerOxygen 5

#define Type_Absolute_Amount 6

#define ATP_Swelling_Factor 0.0000001f 

#define BuildingCost_Factor 1.5f
#define BuildingCost_DNA_Factor 2.0f
#define BuildingCost_OxygenSusceptibility_Factor 0.25f

#define Oxygen_Damage_Factor 0.0025

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

	bool isAlive = true;
	bool isSelected = false;

	float volume;
	float surfaceArea;
	int chunkSize;
	
	float size = 0.0f;
	float length = 0.0f;
	float swellPercent = 1.0f; //if this reaches 1.25 then the cell dies

	int timeAlive = 0;

	unsigned long parentID = 0;
	unsigned long ID = 0;
	int generation = 0;

	bool hasMembrane = false;
	bool hasEnergyManager = false;
	bool hasSplitter = false;

	float ATP = 0;
	float oxygenSusceptiblity = 0.0f;
	float totalPoisonDamage = 0.0f;

	float buildingCost = 0;
	float initialBuildingCost = 0;

	XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };

	int filterColourCount = 0;
	XMFLOAT4* filterColour;

	vector<NeuralNetworkInput*> neuralInps;
	vector<Trait*> traits;

	const static string dnaCriteria[];

public:
	Cell(RenderClass* rndCls, World* world, DNA* dna = nullptr, Cell* pCell = nullptr);
	Cell(RenderClass* rndCls, World* world, DNA* dna, float x, float y, float z);

	void Tick(float t);
	void CheckDNAForTraits();

	unsigned long GetID() { return ID; }
	unsigned long GetParentID() { return parentID; }
	void SetSelected(bool sel);

	World* GetWorld() { return world; }
	bool BuildCell(float buildAmount) { buildingCost -= buildAmount; return buildingCost <= 0; }
	float GetBuildingCost() { return buildingCost; }
	float GetATP() { return ATP; }
	void SetATP(float a) { ATP = a; }
	float LimitATPUsage(__in float projectedATPUsage, __in float Surface, __out float* modifier);
	float LimitATPUsage(__in float projectedATPUsage, __in float Surface);
	float GetSurfaceArea() { return surfaceArea; }
	float GetTimeAlive() { return timeAlive; }

	float GetVolume() { return volume; }

	DNA* GetDNA() { return dna; }

	Model* GetModel() { return mod; }
	RenderClass* GetRenderClass() { return render; }
	ChemicalContainer* GetChemCon() { return chemCon; }

	XMFLOAT4* GetPosition() { return mod->GetPosition(); }
	float GetPositionX() { return mod->GetPosition()->x; }
	float GetPositionY() { return mod->GetPosition()->y; }
	float GetPositionZ() { return mod->GetPosition()->z; }

	float GetSquaredDistanceToClosestCell();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT4* pos) { SetPosition(pos->x, pos->y, pos->z); }
	void AddPosition(float x, float y, float z);

	float GetSpeedX() { return velocity.x; }
	float GetSpeedY() { return velocity.y; }
	float GetSpeedZ() { return velocity.z; }

	void SetVelocity(XMFLOAT3* vel) { velocity = *vel; }
	void AddVelocity(XMFLOAT3* vel) { velocity.x += vel->x; velocity.y += vel->y; velocity.z += vel->z; }
	void AddVelocity(float x, float y, float z) { velocity.x += x; velocity.y += y; velocity.z += z; }
	XMFLOAT3* GetVelocity() { return &velocity; }

	float GetSize() { return size; }
	float GetLength() { return length; }

	void AddToSwellPercent(float add) { swellPercent += add; }
	float GetSwellPercent() { return swellPercent; }

	float GetOxygenSusceptibility() { return oxygenSusceptiblity; }

	void SetMembraneStatus(bool mem) { hasMembrane = mem; }
	Chunk* GetCurrentChunk() { return chunk; }

	NeuralNetwork* GetNeuralNetwork() { return neuralNet; }

	SplittingManager* GetSplittingManager();

	void AddToDNAColourX(int index, float add) { filterColour[index].x += add; }
	void AddToDNAColourY(int index, float add) { filterColour[index].y += add; }
	void AddToDNAColourZ(int index, float add) { filterColour[index].z += add; }

	void ReleaseCell(Cell* pCell);
	void Die(bool explode = false, bool faultyCell = false);

	void ForceSplit();
	string GetOutputString();
	void AddCountForOutput(CellInformation *cellInfo);

	~Cell();
};

#endif