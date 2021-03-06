#include "CellInfoWindow.h"
#include "include.h"
#include "world.h"
#include "Cell.h"
#include "RenderClass.h"
#include "Camera.h"
#include "ChemicalContainer.h"
#include "Chunk.h"
#include "NeuralNetwork.h"

CellInfoWindow::CellInfoWindow(HINSTANCE hIns, World* w, RenderClass* render)
{
	hInstance = hIns;
	world = w;
	this->render = render;
	cam = render->GetCamera();

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "eveCellInfo";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Call to RegisterClassEx on info failed!", "Eve", NULL);
	}

	hWnd = CreateWindow("eveCellInfo", "Eve Cell Info", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 0, 0, 350, 1000, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, "Call to CreateWindow on info failed!", "Eve", NULL);
	}

	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	SetWindowPos(hWnd, HWND_TOP, desktop.right - 25 - 350, 25, 0, 0, SWP_NOSIZE || SWP_SHOWWINDOW);

	RECT rect;
	GetWindowRect(hWnd, &rect);
	textLabel = CreateWindow("STATIC", "not initilized", WS_VISIBLE | WS_CHILD | SS_LEFT, 0, 0, rect.right, rect.bottom, hWnd, NULL, hInstance, NULL);
	WriteInfoData();

	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);
}

void CellInfoWindow::SetClosestCellAsCurrentCell()
{
	//yes the real distance is the root of d but since we are just comparing this is equivalent to comparing the real distance
	float d = -1;
	float cd = 0;

	XMFLOAT4* camPos = cam->GetPosition();
	Cell* newCCell = nullptr;

	for (Cell* c : *world->GetCellVec())
	{
		cd = pow(c->GetPositionX() - camPos->x, 2) + pow(c->GetPositionY() - camPos->y, 2) + pow(c->GetPositionZ() - camPos->z, 2);
		if (d == -1 || cd < d)
		{
			d = cd;
			newCCell = c;
		}
	}

	SetCellAsCurrentCell(newCCell);
}

void CellInfoWindow::SetCellAsCurrentCell(Cell* newCCell)
{
	if (currentCell)
	{
		currentCell->SetSelected(false);
	}

	currentCell = newCCell;

	if (newCCell)
	{
		newCCell->SetSelected(true);
	}
}

void CellInfoWindow::WriteInfoData()
{
	string buffer = "";

	if (currentCell != nullptr)
	{
		buffer += currentCell->GetOutputString();
	}

	SetWindowText(textLabel, TEXT(buffer.c_str()));
}

LRESULT CellInfoWindow::Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_PAINT:
	{
		//world->WriteInfoData();
		break;
	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

CellInfoWindow::~CellInfoWindow()
{
}
