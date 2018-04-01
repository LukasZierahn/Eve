#ifndef __H__include
#define __H__include

#include <wrl/client.h>
#include <vector>
#include <map>
#include <string>
#include <Windowsx.h>
#include <d3d11_1.h>
#include <directxmath.h>
#include <D3Dcompiler.h>
#include <directxcolors.h>
#include <fstream>
#include <tchar.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

void static CheckForError(HRESULT hr, string mes)
{
	if (FAILED(hr))
	{
		MessageBox(NULL, mes.c_str(), NULL, MB_OK);
	}
}

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT2 texturePos;
};

struct ConstantBuffer
{
	XMMATRIX worldPos;
	XMMATRIX worldRot;
	XMMATRIX view;
	XMMATRIX projection;
};

struct  ModelBuffer
{
	XMFLOAT4X4 scale;
	XMFLOAT4X4 position;
	XMFLOAT4X4 rotation;
	XMFLOAT4 dnaInducedColour; //this is needed so that cells look relative to their dna
};

struct ModelData
{
	bool initialized = false;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	int vertexCount;
	int	indexCount;
};

struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

const static int ConvertDNAtoInt(char c)
{
	return (c - 32); //there are 32 characters in the front of the ASCII table that I wont use in the DNA string
}

const static float Balance(float x)
{

	while (x >= XM_2PI)
	{
		x -= XM_2PI;
	}

	while (x < 0)
	{
		x += XM_2PI;
	}

	return x;
}

const static float BalanceAndLock(float x)
{
	if (x > XM_PI)
		x = XM_PI;

	if (x < -XM_PI)
		x = -XM_PI;

	return x;
}

const static void TurnRadiantsIntoXandZComponent(float Rotation, float *x, float *z)
{
	if (Rotation <= XM_PIDIV2)
	{
		*z = (Rotation / -XM_PIDIV2) + 1;
		*x = 1 - *z;
	}
	else if (Rotation <= XM_PI)
	{
		*z = (Rotation / -XM_PIDIV2) + 1;
		*x = (Rotation / -XM_PIDIV2) + 2;
	}
	else if (Rotation <= XM_PIDIV2 * 3)
	{
		*z = (Rotation / XM_PIDIV2) - 3;
		*x = (Rotation / -XM_PIDIV2) + 2;
	}
	else
	{
		*z = (Rotation / XM_PIDIV2) - 3;
		*x = (Rotation / XM_PIDIV2) - 4;
	}
}

const static void TurnRadiantsIntoXandZComponentSin(float Rotation, float *x, float *z)
{
	*z = cos(Rotation);
	*x = sin(Rotation);
}

#endif