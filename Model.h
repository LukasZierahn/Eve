#ifndef __H__Model
#define __H__Model

#include "Include.h"

class RenderClass;
class Texture;

//yes it would have been way more neat to actually inherit Models but as I overtook the code from an older project; i kept this quirk
//due to time constrains, as it is quiet an sizeable change to meddle with every line in which the Model class gets called.
//I did not fully grasped abstract classes at the time i wrote it and admittetly also at the time i started the project, today i would do it differently.

//defining a Model that will (not) be inherited
class Model
{
public:
	Model(RenderClass*);
	Model(RenderClass*, float, float, float);
	~Model();

	void SetPosition(float, float, float);
	void SetPosition(XMFLOAT4);
	void AddPosition(float, float, float, bool = false);

	void SetRotation(float, float, float);
	void AddRotation(float, float, float);
	XMFLOAT3 GetRotation() { return XMFLOAT3(roll, pitch, yawn); }


	void SetScale(float, float, float);

	XMFLOAT4X4* GetScale() { return &scale; }

	void SetData(ModelData* d) { data = d; };
	ModelData** GetDataPointer() { return &data; };
	void SetTexture(Texture* t) { tex = t; };
	Texture** GetTexturePointer() { return &tex; };

	XMFLOAT4* GetPosition() { return &position; }
	XMFLOAT4 GetBoundingBox(); //this an XMFloat4, Add and Subtract each Value to the corresponding value from the current model positiong to get a two points for the bounding box

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

	float biggestScale = 0.0f;

protected:
	XMFLOAT4 position;
	XMFLOAT4X4 scale;

	float roll = 0.0f;
	float pitch = 0.0f;
	float yawn = 0.0f;
};

#endif