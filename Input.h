#ifndef __H__Input
#define __H__Input

#include "Include.h"

class RenderClass;
class Model;

class Input
{
public:
	Input(RenderClass*, RECT);
	void Key(bool, WPARAM);
	void Mouse(WPARAM, int, int, HWND hWnd);

	void RunTick(int t);

private:
	RenderClass* render;
	RECT wndRect;

	Model* mod;

	float speed = 0.005f;

	float mouseSense = 500.0f;

	bool keyW = false;
	bool keyA = false;
	bool keyS = false;
	bool keyD = false;

	bool keyC = false;
	bool keyV = false;

	float x, y, z;
	float rX, rZ;
};

#endif