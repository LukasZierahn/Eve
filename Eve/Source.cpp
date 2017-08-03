#include "include.h"
#include "RenderClass.h"
#include "Cell.h"
#include "Model.h"
#include "Input.h"
#include "World.h"

RenderClass* render;
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

LRESULT CALLBACK InfoProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	//the window containing information about the current chunk and cells (we pro recycling)
	wcex.lpszClassName = "eveInfo";
	wcex.lpfnWndProc = InfoProc;
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Call to RegisterClassEx on info failed!", "Eve", NULL);
		return 1;
	}

	HWND infoHWnd = CreateWindow("eveInfo", "Eve Info", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 0, 0, 350, 1000, NULL, NULL, hInstance, NULL);

	if (!infoHWnd)
	{
		MessageBox(NULL, "Call to CreateWindow on info failed!", "Eve", NULL);
		return 1;
	}

	world = new World(render, infoHWnd, hInstance, 50, 10);

	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);	
	
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	SetWindowPos(hWnd, HWND_TOP, (desktop.right / 2) - 600, (desktop.bottom / 2) - 375, 0, 0, SWP_NOSIZE || SWP_SHOWWINDOW);

	UpdateWindow(infoHWnd);

	world->AddCell(0.0f, 0.0f, 0.0f);
	world->AddCell(0.0f, 0.0f, 0.0f);

	//Initalising the Message loop
	MSG Msg;
	ZeroMemory(&Msg, sizeof(MSG));

	long long t = milliseconds_now();

	while (true)
	{
		long long NewTime = milliseconds_now();
		int DeltaTime = NewTime - t;

		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);

			if (Msg.message == WM_QUIT || (Msg.message == WM_KEYDOWN && Msg.wParam == VK_ESCAPE))
				break;
		}

		render->GetInput()->RunTick(DeltaTime);
		render->RenderFrame();

		t = NewTime;
	}

	delete render, world;

	return 0;
}