#include "HardwareAcceleration.h"



HardwareAcceleration::HardwareAcceleration()
{
	HRESULT hr;

	D3D_FEATURE_LEVEL directXFeatureLvl[] = { D3D_FEATURE_LEVEL_11_0 };

	hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, directXFeatureLvl, ARRAYSIZE(directXFeatureLvl), D3D11_SDK_VERSION, device.GetAddressOf(), NULL, deviceContext.GetAddressOf());

	//compiling and setting the shaders
	ID3D10Blob *errorBlob;
	ID3D10Blob *fluidMovementBlob;

	hr = D3DCompileFromFile(L"hardwareAccelerationShader.HLSL", NULL, NULL, "HAS", "gs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL1 || D3DCOMPILE_WARNINGS_ARE_ERRORS, NULL, &fluidMovementBlob, &errorBlob);
	CheckForError(hr, "Failed to fill VertexShaderBlob");

	hr = device->CreateVertexShader(fluidMovementBlob->GetBufferPointer(), fluidMovementBlob->GetBufferSize(), NULL, fluidMovementShader.GetAddressOf());

	//defining an input layout
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "position", 0, DXGI_FORMAT_D32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), fluidMovementBlob->GetBufferPointer(), fluidMovementBlob->GetBufferSize(), inputLayout.GetAddressOf());
	deviceContext->IASetInputLayout(inputLayout.Get());

	fluidMovementBlob->Release();
}


HardwareAcceleration::~HardwareAcceleration()
{
}
