#include "Include.h"
#include "Model.h"
#include "RenderClass.h"
#include "Texture.h"

Model::Model(RenderClass* rndCls)
{
	render = rndCls;

	render->GetDevice(&device);
	render->GetDeviceContext(&deviceContext);
	render->GetSamplerState(&samplerState);

	scale = XMMatrixScaling(1,1,1);

	modBufHeader.position = XMMatrixIdentity();
	modBufHeader.rotation = XMMatrixIdentity();
	modBufHeader.scale = XMMatrixTranspose(scale);

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

	position = XMVectorSet(x, y, z, 1);
}

void Model::Draw()
{
	if (data != nullptr && data->initialized)
	{
		//buffer stuff
		modBufHeader.position = XMMatrixTranslationFromVector(position);
		modBufHeader.rotation = XMMatrixRotationRollPitchYaw(roll, pitch, yawn);

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

void Model::Setposition(float x, float y, float z)
{
	position = XMVectorSet(x, y, z, 1);
}

void Model::Setposition(XMVECTOR newpos)
{
	position = newpos;
}

void Model::Addposition(float x, float y, float z, bool collision)
{
		XMVECTOR BufferVector = XMVectorSet(x, y, z, 1);
		position += BufferVector;
}

Model::~Model()
{
}