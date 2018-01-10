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

	void SetPosition(float, float, float);
	void SetPosition(XMFLOAT4);
	void AddPosition(float, float, float, bool = false);

	void SetRotation(float, float, float);
	void AddRotation(float, float, float);

	void SetScale(float, float, float);

	void SetData(ModelData* d) { data = d; };
	ModelData** GetDataPointer() { return &data; };
	void SetTexture(Texture* t) { tex = t; };
	Texture** GetTexturePointer() { return &tex; };

	XMFLOAT4* GetPosition() { return &position; }

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
	XMFLOAT4 position;
	XMFLOAT4X4 scale;

	float roll = 0.0f;
	float pitch = 0.0f;
	float yawn = 0.0f;
};

#endif