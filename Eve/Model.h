#ifndef __H__Model
#define __H__Model

#include "Include.h"

class RenderClass;
class Texture;

//defining a Model that will be inherited
class Model
{
public:
	Model() { };
	Model(RenderClass*);
	Model(RenderClass*, float, float, float);
	~Model();

	void SetRotation(float, float, float);
	void AddRotation(float, float, float);
	void Setposition(float, float, float);
	void Setposition(XMVECTOR);
	void Addposition(float, float, float, bool = false);

	void SetData(ModelData* d) { data = d; };
	ModelData** GetDataPointer() { return &data; };
	void SetTexture(Texture* t) { tex = t; };
	Texture** GetTexturePointer() { return &tex; };

	XMVECTOR* Getposition() { return &position; }

	void Draw();

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<ID3D11SamplerState> samplerState;

	RenderClass* render = nullptr;

	ComPtr<ID3D11Buffer> modBuf;

	ModelData* data = nullptr;

	Texture* tex = nullptr;

	ModelBuffer modBufHeader;

protected:
	XMVECTOR position;
	XMMATRIX scale;

	float roll = 0.0f;
	float pitch = 0.0f;
	float yawn = 0.0f;
};

#endif