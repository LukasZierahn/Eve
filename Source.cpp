#include "include.h"
#include "RenderClass.h"
#include "Cell.h"
#include "Model.h"
#include "Input.h"
#include "World.h"
#include "InfoWindow.h"
#include "Chunk.h"
#include "ChemicalContainer.h"
#include "Camera.h"
#include "CellInfoWindow.h"

RenderClass* render = nullptr;
InfoWindow* infoWindow = nullptr;
CellInfoWindow* cellInfoWindow = nullptr;
World* world = nullptr;

LRESULT CALLBACK EveProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		render->GetInput()->Key(true, wParam);
		break;

	case WM_KEYUP:
		render->GetInput()->Key(false, wParam);
		break;

	case WM_MOUSEMOVE:
		render->GetInput()->Mouse(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), hWnd);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

long long milliseconds_now() 
{
	LARGE_INTEGER frequency;
	BOOL use_qpc = QueryPerformanceFrequency(&frequency);

	if (use_qpc)
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / frequency.QuadPart;
	}
	else
	{
		return GetTickCount();
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CMDLine, int CmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = EveProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "eve";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	//the main DirectX window
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Call to RegisterClassEx failed!", "Eve", NULL);
		return 1;
	}

	HWND hWnd = CreateWindow("eve", "Eve", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 750, NULL, NULL, hInstance, NULL);
	render = new RenderClass(hWnd);

	if (!hWnd)
	{
		MessageBox(NULL, "Call to CreateWindow failed!", "Eve", NULL);
		return 1;
	}

	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	SetWindowPos(hWnd, HWND_TOP, (desktop.right / 2) - 600, (desktop.bottom / 2) - 375, 0, 0, SWP_NOSIZE || SWP_SHOWWINDOW);

	world = new World(render, 50, 10);
	render->GetCamera()->SetWorld(world);
	render->SetWorld(world);
	infoWindow = new InfoWindow(hInstance, world);
	cellInfoWindow = new CellInfoWindow(hInstance, world, render);

	render->GetInput()->SetCellInfoWindow(cellInfoWindow);

	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);	

	world->AddCell(new Cell(render, world, 50.0f, 50.0f, 50.0f));

	//Initalising the Message loop
	cellInfoWindow->SetClosestCellAsCurrentCell();
	MSG Msg;
	ZeroMemory(&Msg, sizeof(MSG));

	long long t = milliseconds_now();
	long long NewTime;
	int DeltaTime;

	int counterForInfoWindowUpdates = 0;
	int FPS = 0;

	while (true)
	{
		NewTime = milliseconds_now();
		DeltaTime = NewTime - t;
		counterForInfoWindowUpdates += DeltaTime;
		FPS++;

		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);

			if (Msg.message == WM_QUIT || (Msg.message == WM_KEYDOWN && Msg.wParam == VK_ESCAPE))
				break;
		}

		render->GetInput()->RunTick(DeltaTime);
		world->Tick(DeltaTime);
		render->RenderFrame();

		if (counterForInfoWindowUpdates > 500)
		{
			infoWindow->WriteInfoData(FPS * (1000.0f / counterForInfoWindowUpdates));
			cellInfoWindow->WriteInfoData();
			counterForInfoWindowUpdates = 0;
			FPS = 0;
		}

		t = NewTime;
	}

	delete render, infoWindow, cellInfoWindow, world;

	return 0;
}