#ifndef __H__InfoWindow
#define __H__InfoWindow

#include "include.h"
class World;

class InfoWindow
{
private:
	HWND hWnd;
	HWND textLabel;
	HINSTANCE hInstance;

	World* world;

public:
	InfoWindow(HINSTANCE hInstance, World* w);

	void WriteInfoData(int FPS = 0);
	static LRESULT CALLBACK Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	~InfoWindow();
};

#endif