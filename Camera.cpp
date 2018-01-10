#include "Camera.h"
#include "Include.h"
#include "RenderClass.h"
#include "World.h"

#include "Cell.h"

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
	eyePos = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	focusPos = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	upDir = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&view, XMMatrixLookAtLH(XMLoadFloat4(&eyePos), XMLoadFloat4(&focusPos), XMLoadFloat4(&upDir)));

	XMStoreFloat4x4(&projection, XMMatrixPerspectiveFovLH(XM_PIDIV4, size.right / (FLOAT)size.bottom, 0.001f, 200.0f));

	deviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());

	SetPosition(0.0f, 0.0f, 0.0f, false);
}

void Camera::UpdateConstBuffer()
{
	ConstantBuffer cb;

	cb.worldRot = XMMatrixIdentity();
	cb.worldPos = XMMatrixIdentity();
	cb.view = XMMatrixTranspose(XMLoadFloat4x4(&view));
	cb.projection = XMMatrixTranspose(XMLoadFloat4x4(&projection));

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

void Camera::SetPosition(float x, float y, float z, bool bounds)
{
	if (bounds)
	{
		world->KeepPointInBounds(&x, &y, &z);
	}

	pos = XMFLOAT4(x, y, z, 1);

	UpdateViewMatrix();
}

void Camera::AddPosition(float x, float y, float z, bool bounds)
{
	pos.x += x;
	pos.y += y;
	pos.z += z;

	if (bounds)
	{
		world->KeepPointInBounds(&x, &y, &z);
	}

	UpdateViewMatrix();
}

void inline Camera::UpdateViewMatrix()
{
	TurnRadiantsIntoXandZComponentSin(pitch, &rX, &rZ);

	if (currentCell)
	{
		pos = *currentCell->GetPosition();

		eyePos = XMFLOAT4(pos.x + (rX * eyePositionToCellScaling), pos.y + (roll * eyePositionToCellScaling) / XM_PIDIV2, pos.z + (rZ * eyePositionToCellScaling), 1);

		focusPos = pos;
	}
	else
	{
		eyePos = pos;

		focusPos = XMFLOAT4(pos.x + rX, pos.y + roll / -XM_PIDIV2, pos.z + rZ, 1);
	}

	XMStoreFloat4x4(&view, XMMatrixLookAtLH(XMLoadFloat4(&eyePos), XMLoadFloat4(&focusPos), XMLoadFloat4(&upDir))); //shhhh we dont talk about how ineffecient this is
}

Camera::~Camera()
{
}
