#ifndef __H__Camera
#define __H__Camera

#include "Include.h"

class RenderClass;
class World;
class Cell;

class Camera
{

private:
	RenderClass* render;
	World* world;

	D3D11_VIEWPORT viewport;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<ID3D11Buffer> constBuffer;

	RECT size;

	XMFLOAT4 pos;

	float roll = 0.0f;
	float pitch = 0.0f;
	float yawn = 0.0f;

	float rX, rZ;

	Cell* currentCell = nullptr;
	float eyePositionToCellScaling = 25.0f;

	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;

	XMFLOAT4 eyePos;
	XMFLOAT4 focusPos;
	XMFLOAT4 upDir;

public:
	Camera(RenderClass*, RECT);
	void UpdateConstBuffer();

	void StopFollowingCell() { currentCell = nullptr; SetPosition(pos.x + (rX * eyePositionToCellScaling), pos.y + (roll * eyePositionToCellScaling) / XM_PIDIV2, pos.z + (rZ * eyePositionToCellScaling)); }
	void FollowCell(Cell* c) { currentCell = c; }
	bool IsFollowingACell() { return currentCell; }

	void SetRotation(float, float, float);
	void AddRotation(float, float, float);
	void SetPosition(float, float, float, bool bounds = true);
	void AddPosition(float, float, float, bool bounds = true);

	XMFLOAT4* GetPosition() { return &pos; }
	void SetWorld(World* w) { world = w; }

	void inline UpdateViewMatrix();

	inline float GetRoll() { return roll; }
	inline float GetPitch() { return pitch; }
	inline float GetYawn() { return yawn; }

	~Camera();
};

#endif