#include "Include.h"
#include "Model.h"
#include "RenderClass.h"
#include "Texture.h"
#include "World.h"

Model::Model(RenderClass* rndCls)
{
	render = rndCls;

	render->GetDevice(&device);
	render->GetDeviceContext(&deviceContext);
	render->GetSamplerState(&samplerState);

	XMStoreFloat4x4(&scale, XMMatrixScaling(1,1,1));

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ModelBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = device->CreateBuffer(&bd, nullptr, modBuf.GetAddressOf());
}

Model::Model(RenderClass* rndCls, float x, float y, float z)
{
	Model::Model(rndCls);

	position = XMFLOAT4(x, y, z, 1);
}

void Model::Draw()
{
	if (data != nullptr && data->initialized)
	{
		//buffer stuff
		XMStoreFloat4x4(&modBufHeader.position, XMMatrixTranslationFromVector(XMLoadFloat4(&position)));
		XMStoreFloat4x4(&modBufHeader.rotation, XMMatrixRotationRollPitchYaw(roll, pitch, yawn));
		modBufHeader.scale = scale;

		D3D11_MAPPED_SUBRESOURCE MappedResource;
		ZeroMemory(&MappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		deviceContext->Map(modBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);

		memcpy(MappedResource.pData, &modBufHeader, sizeof(modBufHeader));

		deviceContext->Unmap(modBuf.Get(), 0);

		deviceContext->VSSetConstantBuffers(1, 1, modBuf.GetAddressOf());

		//drawing stuff
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, data->vertexBuffer.GetAddressOf(), &stride, &offset);

		deviceContext->IASetIndexBuffer(data->indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());

		tex->SetTextureAsShaderRessource();

		deviceContext->DrawIndexed(data->indexCount, 0, 0);
	}
}

void Model::SetRotation(float R, float P, float Y)
{
	roll = R;
	pitch = P;
	yawn = Y;
}

void Model::AddRotation(float R, float P, float Y)
{
	roll += R;
	pitch += P;
	yawn += Y;
}

void Model::SetScale(float x, float y, float z)
{
	XMFLOAT4X4 scl;
	ZeroMemory(&scl, sizeof(XMFLOAT4X4));

	scl._11 = x;
	scl._22 = y;
	scl._33 = z;
	scl._44 = 1;

	scale = scl;
}

void Model::SetPosition(float x, float y, float z)
{
	position = XMFLOAT4(x, y, z, 1);
}

void Model::SetPosition(XMFLOAT4 newpos)
{
	position = newpos;
}

void Model::AddPosition(float x, float y, float z, bool collision)
{
	position.x += x;
	position.y += y;
	position.z += z;

	render->GetWorld()->KeepPointInBounds(&position.x, &position.y, &position.z);
}

Model::~Model()
{
}