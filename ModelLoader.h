#ifndef __H__ModelLoader
#define __H__ModelLoader

#include "Include.h"


class RenderClass;
class Texture;
class DNA;

class ModelLoader
{
public:
	ModelLoader(RenderClass*);
	void GetModel(string name, ModelData**, bool = false);
	void GetTexture(string name, Texture**);

	~ModelLoader();

private:
	RenderClass* render;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	bool LoadModel(string name, vector<Vertex>*, vector<WORD>*, bool);
	bool CreateModel(string name);

	map<string, ModelData*> modelDataMap;

	map<string, Texture*> textureMap;

	Texture* errorText;
};

#endif
