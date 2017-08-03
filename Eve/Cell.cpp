#include "Cell.h"
#include "Model.h"
#include "RenderClass.h"
#include "include.h"
#include "ModelLoader.h"


Cell::Cell(RenderClass* rndCls)
{
	mod = new Model(rndCls);

	rndCls->GetModelLoader()->GetModel("Cube", mod->GetDataPointer());
	rndCls->GetModelLoader()->GetTexture("Cube", mod->GetTexturePointer());

	rndCls->AddModel(mod);
}

Cell::Cell(RenderClass* rndCls, float x, float y, float z) : Cell(rndCls)
{
	mod->Setposition(x, y, z);
}


Cell::~Cell()
{
	delete mod;
}
