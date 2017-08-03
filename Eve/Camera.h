#pragma once
#include "Include.h"

#ifndef __H__Camera
#define __H__Camera

class RenderClass;

class Camera
{
public:
	Camera(RenderClass*, RECT);
	void UpdateConstBuffer();

	void SetRotation(float, float, float);
	void AddRotation(float, float, float);
	void SetPosition(float, float, float);
	void AddPosition(float, float, float);

	XMVECTOR* GetPosition() { return &pos; }

	void inline UpdateViewMatrix();

	inline float GetRoll() { return roll; }
	inline float GetPitch() { return pitch; }
	inline float GetYawn() { return yawn; }

	~Camera();

private:
	RenderClass* render;

	D3D11_VIEWPORT viewport;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<ID3D11Buffer> constBuffer;

	RECT size;

	XMVECTOR pos;

	float roll = 0.0f;
	float pitch = 0.0f;
	float yawn = 0.0f;

	float rX, rZ;

	XMMATRIX view;
	XMMATRIX projection;

	XMVECTOR eyePos;
	XMVECTOR focusPos;
	XMVECTOR upDir;
};

#endif