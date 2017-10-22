#include "include.h"
#include "ModelLoader.h"
#include "RenderClass.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"

RenderClass::RenderClass(HWND wndhandle)
{
	hwnd = wndhandle;
	GetClientRect(hwnd, &size);
	HRESULT hr;

	DXGI_RATIONAL refreshRateRat; //A rational (Numerator/Denominator) describing the framerate and it will get bound into the displaymode and eventually into the swapChain
	refreshRateRat.Numerator = 120;
	refreshRateRat.Denominator = 1;

	DXGI_MODE_DESC ModeDesc; //A displaymode in order to be bound into the swapChain
	ZeroMemory(&ModeDesc, sizeof(ModeDesc));
	ModeDesc.Width = size.right;
	ModeDesc.Height = size.bottom;
	ModeDesc.RefreshRate = refreshRateRat;
	ModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //The way our pixel are definded
	ModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE; //If we think about the screen as an array of pixels, every horizontal row of pixels is a scanline in the big array
	ModeDesc.Scaling = DXGI_MODE_SCALING_CENTERED; //We dont scale

	DXGI_SAMPLE_DESC dMultiSampleing; //Determinting the MultiSampleing
	dMultiSampleing.Count = 1;
	dMultiSampleing.Quality = 0;

	DXGI_SWAP_CHAIN_DESC swapChainDesc; //Determinting the properties of the swapChain that will be created with the device, it is not the swapChain itself
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc = ModeDesc;
	swapChainDesc.SampleDesc = dMultiSampleing;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL directXFeatureLvl[] = { D3D_FEATURE_LEVEL_11_0 };

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, directXFeatureLvl, ARRAYSIZE(directXFeatureLvl), D3D11_SDK_VERSION, &swapChainDesc, swapChain.GetAddressOf(), device.GetAddressOf(), NULL, deviceContext.GetAddressOf());

	//create a render target view
	ID3D11Texture2D* backBufferRessource;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferRessource);

	device->CreateRenderTargetView(backBufferRessource, NULL, backbuffer.GetAddressOf());

	D3D11_TEXTURE2D_DESC backBufferDesc;
	backBufferRessource->GetDesc(&backBufferDesc);

	backBufferRessource->Release();

	ID3D11Texture2D* depthStencilTexture;
	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Width = backBufferDesc.Width;
	depthDesc.Height = backBufferDesc.Height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthDesc.SampleDesc = dMultiSampleing;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	device->CreateTexture2D(&depthDesc, NULL, &depthStencilTexture);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	// Depth test parameters
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());

	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, depthStencilView.GetAddressOf());

	deviceContext->OMSetRenderTargets(1, backbuffer.GetAddressOf(), depthStencilView.Get());

	//compiling and setting the shaders
	ID3D10Blob *errorBlob;
	ID3D10Blob *vertexShaderBlob, *pixelShaderBlob;

	hr = D3DCompileFromFile(L"shader.HLSL", NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL1 || D3DCOMPILE_WARNINGS_ARE_ERRORS, NULL, &vertexShaderBlob, &errorBlob);
	CheckForError(hr, "Failed to fill VertexShaderBlob");
	hr = D3DCompileFromFile(L"shader.HLSL", NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL1 || D3DCOMPILE_WARNINGS_ARE_ERRORS, NULL, &pixelShaderBlob, &errorBlob);
	CheckForError(hr, "Failed to fill PixelShaderBlob");

	hr = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, vertexShader.GetAddressOf());
	hr = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, pixelShader.GetAddressOf());

	//defining an input layout
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXPOS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), inputLayout.GetAddressOf());
	deviceContext->IASetInputLayout(inputLayout.Get());

	vertexShaderBlob->Release();
	pixelShaderBlob->Release();

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());
	deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());

	SetFillMode(D3D11_FILL_SOLID);

	UpdateWindow(hwnd);

	RECT rect;
	GetWindowRect(hwnd, &rect);

	modLoad = new ModelLoader(this);
	cam = new Camera(this, size);
	inp = new Input(this, rect);
}

void RenderClass::RenderFrame()
{
	float colour[4] = { 0.5f, 0.1f, 0.7f, 1.0f };
	deviceContext->ClearRenderTargetView(backbuffer.Get(), colour);

	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.0f, 0);

	cam->UpdateConstBuffer();

	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	for (unsigned short i = 0; i < modelVector.size(); i++)
	{
		modelVector[i]->Draw();
	}

	swapChain->Present(0, 0);
}

void RenderClass::SetFillMode(D3D11_FILL_MODE fillmode)
{
	D3D11_RASTERIZER_DESC rasDesc;
	ZeroMemory(&rasDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasDesc.FillMode = fillmode;
	rasDesc.CullMode = D3D11_CULL_BACK;
	rasDesc.DepthClipEnable = true;
	rasDesc.ScissorEnable = false;
	rasDesc.FrontCounterClockwise = true;

	device->CreateRasterizerState(&rasDesc, rasState.GetAddressOf());

	deviceContext->RSSetState(rasState.Get());
}

RenderClass::~RenderClass()
{
	swapChain->SetFullscreenState(false, NULL);

	delete modLoad;
	delete inp;
	delete cam;

	for (unsigned short i = 0; i < modelVector.size(); i++)
	{
		delete modelVector[i];
	}
}