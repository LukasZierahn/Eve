#ifndef __H__RenderClass
#define __H__RenderClass

#include "include.h"

class Model;
class ModelLoader;
class Camera;
class Input;
class World;

#define Amount_of_Cell_Display_Modes 3

class RenderClass
{
public:
	RenderClass(HWND);

	void SetWorld(World* w) { world = w; }

	void RenderFrame();

	Model* GetModel(int index) { return modelVector[index]; }
	void AddModel(Model* mod) { modelVector.push_back(mod); modelVector.shrink_to_fit(); }

	void GetDevice(ComPtr<ID3D11Device>* dev) { device.As(dev); }
	void GetDeviceContext(ComPtr<ID3D11DeviceContext>* devcon) { deviceContext.As(devcon); }
	void GetRasDesc(D3D11_RASTERIZER_DESC* rasDesc) { rasState->GetDesc(rasDesc); }
	void GetSamplerState(ComPtr<ID3D11SamplerState>* sample) { samplerState.As(sample); }

	ModelLoader* GetModelLoader() { return modLoad; }
	Input* GetInput() { return inp; }
	Camera* GetCamera() { return cam; }

	int ModelCount() { return modelVector.size(); }

	World* GetWorld() { return world; }

	void SetFillMode(D3D11_FILL_MODE);

	void CycleCellDisplayMode()
	{
		cellDisplayMode = (cellDisplayMode + 1) % Amount_of_Cell_Display_Modes;
	}

	short GetCellDisplayMode() { return cellDisplayMode; }

	~RenderClass();

private:
	HWND hwnd;

	vector<Model*> modelVector;

	RECT size;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> backbuffer;
	ComPtr<ID3D11DepthStencilState> depthStencilState;
	ComPtr<ID3D11DepthStencilView> depthStencilView;

	ComPtr<ID3D11InputLayout> inputLayout;

	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;

	ComPtr<ID3D11SamplerState> samplerState;

	ComPtr<ID3D11RasterizerState> rasState;

	Camera* cam = nullptr;
	Input* inp = nullptr;
	ModelLoader* modLoad = nullptr;

	short cellDisplayMode = 0; //0 is normal Cell texture mode, 1 is Dna mode

	World* world = nullptr;
};

#endif