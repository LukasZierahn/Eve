#include "Texture.h"
#include "RenderClass.h"

Texture::Texture(RenderClass* rend, string filename)
{
	render = rend;

	render->GetDevice(&device);
	render->GetDeviceContext(&deviceContext);

	fullFilename = "Textures\\";
	fullFilename += filename;
	fullFilename += ".tga";

	header = new TargaHeader();

	if (OpenTexture())
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Height = header->height;
		textureDesc.Width = header->width;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		device->CreateTexture2D(&textureDesc, NULL, textureData.GetAddressOf());

		int rowPitch = (header->width * 4) * sizeof(unsigned char);

		deviceContext->UpdateSubresource(textureData.Get(), 0, NULL, data, rowPitch, 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC ShaderRessourceViewDesc;
		ShaderRessourceViewDesc.Format = textureDesc.Format;
		ShaderRessourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		ShaderRessourceViewDesc.Texture2D.MostDetailedMip = 0;
		ShaderRessourceViewDesc.Texture2D.MipLevels = -1;

		device->CreateShaderResourceView(textureData.Get(), &ShaderRessourceViewDesc, shaderRessourceView.GetAddressOf());

		deviceContext->GenerateMips(shaderRessourceView.Get());

		succeded = true;
	}
}

bool Texture::OpenTexture()
{
	FILE* filePointer;

	if (fopen_s(&filePointer, fullFilename.c_str(), "rb") != 0)
		return false;

	fread(header, sizeof(TargaHeader), 1, filePointer);

	if (header->bpp != 32)
		return false;

	imageSize = header->width * header->height * 4;
	unsigned char* Buffer;

	Buffer = new unsigned char[imageSize];
	fread(Buffer, 1, imageSize, filePointer);

	fclose(filePointer);

	data = new unsigned char[imageSize];

	int Index, j, k, i;
	Index = 0;
	k = imageSize - (header->width * 4);

	for (j = 0; j < header->height; j++)
	{
		for (i = 0; i < header->width; i++)
		{
			data[Index + 0] = Buffer[k + 2];  // Red.
			data[Index + 1] = Buffer[k + 1];  // Green.
			data[Index + 2] = Buffer[k + 0];  // Blue
			data[Index + 3] = Buffer[k + 3];  // Alpha

			k += 4;
			Index += 4;
		}

		k -= (header->width * 8);
	}

	delete[] Buffer;
	return true;
}

void Texture::SetTextureAsShaderRessource()
{
	deviceContext->PSSetShaderResources(0, 1, shaderRessourceView.GetAddressOf());
}

Texture::~Texture()
{
	delete header;
	delete[] data;
}
