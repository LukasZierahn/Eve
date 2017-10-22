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

	int WriteString(HDC hDC, string toWrite, int pos);

public:
	InfoWindow(HWND hW, HINSTANCE hInstance, World* w);

	void WriteInfoData();

	~InfoWindow();
};

#endif