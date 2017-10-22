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
	Chunk(int x, int y, int z, World* world, int vol);

	void AquireNeighbours();
	ChemicalContainer* GetChemCon() { return ChemCon; };

	~Chunk();
};

#endif