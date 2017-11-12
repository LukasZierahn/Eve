#include "Cell.h"
#include "Model.h"
#include "RenderClass.h"
#include "include.h"
#include "ModelLoader.h"
#include "ChemicalContainer.h"
#include "World.h"
#include "Chunk.h"

Cell::Cell(RenderClass* rndCls, World* world)
{
	volume = 10000; //this is not finalised

	mod = new Model(rndCls);
	this->world = world;
	chemCon = new ChemicalContainer(world, volume);

	chunkSize = world->GetChunkSize();

	rndCls->GetModelLoader()->GetModel("Cube", mod->GetDataPointer());
	rndCls->GetModelLoader()->GetTexture("Cube", mod->GetTexturePointer());

	rndCls->AddModel(mod);
}

Cell::Cell(RenderClass* rndCls, World* world, float x, float y, float z) : Cell(rndCls, world)
{
	SetPosition(x, y, z);
}

void Cell::Tick(float t)
{
	XMFLOAT4 pos = *mod->GetPosition();

	chunk = world->GetChunk(floor(pos.x / chunkSize), floor(pos.y / chunkSize), floor(pos.z / chunkSize));

	chemCon->DiffuseFromAndTo(chunk->GetChemCon(), t);

	chemCon->ApplyContains();
}


void Cell::SetPosition(float x, float y, float z)
{
	int worldChunkSize = world->GetChunkSize();
	world->KeepPointInBounds(&x, &y, &z);

	mod->SetPosition(x, y, z);

}

void Cell::AddPosition(float x, float y, float z)
{
	XMFLOAT4 pos = *mod->GetPosition();

	x += pos.x;
	y += pos.y;
	z += pos.z;

	int worldChunkSize = world->GetChunkSize();
	world->KeepPointInBounds(&x, &y, &z);

	mod->SetPosition(x, y, z);
}

Cell::~Cell()
{
	delete chemCon, mod;
}
