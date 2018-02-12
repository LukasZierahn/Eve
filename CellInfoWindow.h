#ifndef __H__CellInfoWindow
#define __H__CellInfoWindow

#include "include.h"
#include "Camera.h"
#include "Cell.h"

class World;
class RenderClass;

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
	void SwitchFollowingCurrentCell() 
	{ 
		if (cam->IsFollowingACell())
			cam->StopFollowingCell();
		else
			cam->FollowCell(currentCell);
	}

	void SplitCurrentCell() { currentCell->ForceSplit(); }

	void WriteInfoData();
	static LRESULT CALLBACK Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	~CellInfoWindow();
};

#endif