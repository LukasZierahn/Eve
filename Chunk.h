#ifndef __H__Chunk
#define __H__Chunk

class ChemicalContainer;
class World;

//a Chunk is basicly a ChemicalContainer with coordinates
class Chunk
{
private:
	int x, y, z;
	ChemicalContainer* ChemCon = nullptr;
	World* world = nullptr;

public:
	Chunk(int x, int y, int z, World* world, float vol, float surArea);

	void AquireNeighbours();
	ChemicalContainer* GetChemCon() { return ChemCon; };

	int GetX() { return x; }
	int GetY() { return y; }
	int GetZ() { return z; }

	~Chunk();
};

#endif