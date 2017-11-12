#ifndef __H__CellInfoWindow
#define __H__CellInfoWindow

#include "include.h"
class World;
class Cell;
class RenderClass;
class Camera;

class CellInfoWindow
{
private:
	HWND hWnd;
	HWND textLabel;
	HINSTANCE hInstance;

	World* world;
	RenderClass* render;
	Camera* cam;

	Cell* currentCell = nullptr;

public:
	CellInfoWindow(HINSTANCE hInstance, World* w, RenderClass* render);

	void SetClosestCellAsCurrentCell();

	void WriteInfoData();
	static LRESULT CALLBACK Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	~CellInfoWindow();
};

#endif