#include "Camera.h"
#include "Include.h"
#include "RenderClass.h"

Camera::Camera(RenderClass* rend, RECT si)
{
	render = rend;

	render->GetDevice(&device);
	render->GetDeviceContext(&deviceContext);
	size = si;

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = size.right;
	viewport.Height = size.bottom;
	viewport.MinDepth = 0.0;
	viewport.MaxDepth = 1.0;

	deviceContext->RSSetViewports(1, &viewport);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&bd, nullptr, constBuffer.GetAddressOf());

	//initialize the matrixes
	eyePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	focusPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	upDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	view = XMMatrixLookAtLH(eyePos, focusPos, upDir);

	projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, size.right / (FLOAT)size.bottom, 0.001f, 200.0f);

	deviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());

	SetPosition(0.0f, 0.0f, 0.0f);
}

void Camera::UpdateConstBuffer()
{
	ConstantBuffer cb;

	cb.worldRot = XMMatrixIdentity();
	cb.worldPos = XMMatrixIdentity();
	cb.view = XMMatrixTranspose(view);
	cb.projection = XMMatrixTranspose(projection);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	ZeroMemory(&MappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	deviceContext->Map(constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);

	memcpy(MappedResource.pData, &cb, sizeof(cb));

	deviceContext->Unmap(constBuffer.Get(), 0);
}

void Camera::SetRotation(float R, float P, float Y)
{
	roll = BalanceAndLock(R);
	pitch = Balance(P);
	yawn = Balance(Y);

	UpdateViewMatrix();
}

void Camera::AddRotation(float R, float P, float Y)
{
	roll = BalanceAndLock(roll + R);
	pitch = Balance(pitch + P);
	yawn = Balance(yawn + Y);

	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	pos = XMVectorSet(x, y, z, 1);

	UpdateViewMatrix();
}

void Camera::AddPosition(float x, float y, float z)
{
	XMVECTOR BufferVector = XMVectorSet(x, y, z, 1);
	pos += BufferVector;

	UpdateViewMatrix();
}

void inline Camera::UpdateViewMatrix()
{
	TurnRadiantsIntoXandZComponentSin(pitch, &rX, &rZ);

	eyePos = pos;

	XMVECTOR BufferVector = XMVectorSet(rX, roll / -XM_PIDIV2, rZ, 1);
	focusPos = pos + BufferVector;

	view = XMMatrixLookAtLH(eyePos, focusPos, upDir);
}

Camera::~Camera()
{
}
