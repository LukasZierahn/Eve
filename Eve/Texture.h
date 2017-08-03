#ifndef __H__Texture
#define __H__Texture

#include "Include.h"

class RenderClass;

class Texture
{
public:
	Texture(RenderClass*, string filename);
	bool hasSucceded() { return succeded; }
	void SetTextureAsShaderRessource();
	~Texture();

private:
	bool OpenTexture();

	RenderClass* render;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	ComPtr<ID3D11Texture2D> textureData;
	ComPtr<ID3D11ShaderResourceView> shaderRessourceView;

	string fullFilename;

	int imageSize;
	unsigned char* data;

	TargaHeader* header;

	bool succeded = false;
};

#endif