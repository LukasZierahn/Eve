#ifndef __H__HardwareAcceleration
#define __H__HardwareAcceleration

#include "include.h"

class HardwareAcceleration
{
	RECT size;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	ComPtr<ID3D11InputLayout> inputLayout;

	ComPtr<ID3D11VertexShader> fluidMovementShader;
public:
	HardwareAcceleration();
	~HardwareAcceleration();
};

#endif