#include "InfoWindow.h"
#include "include.h"
#include "world.h"
#include "Camera.h"
#include "RenderClass.h"
#include "Input.h"

InfoWindow::InfoWindow(HINSTANCE hIns, World* w)
{
	hInstance = hIns;
	world = w;
	cam = w->GetRenderClass()->GetCamera();

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
	wcex.lpszClassName = "eveInfo";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Call to RegisterClassEx on info failed!", "Eve", NULL);
	}

	hWnd = CreateWindow("eveInfo", "Eve Info", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 0, 0, 350, 1000, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, "Call to CreateWindow on info failed!", "Eve", NULL);
	}

	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	SetWindowPos(hWnd, HWND_TOP, 25, 25, 0, 0, SWP_NOSIZE || SWP_SHOWWINDOW);


	RECT rect;
	GetWindowRect(hWnd, &rect);
	textLabel = CreateWindow("STATIC", "not initilized", WS_VISIBLE | WS_CHILD | SS_LEFT, 0, 0, rect.right, 600, hWnd, NULL, hInstance, NULL);
	WriteInfoData();

	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);
}

void InfoWindow::WriteInfoData(int FPS)
{
	string buffer = world->GetInfoWindowString();

	XMFLOAT4* camPos = cam->GetPosition();
	buffer += "\n x/y/z: " + to_string(camPos->x) + "/" + to_string(camPos->y) + "/" + to_string(camPos->z) + " \n";
	buffer += " Interfernce Mode: " + to_string(world->GetRenderClass()->GetInput()->GetInterferenceMode()) + " \n";
	buffer += " Fps: " + to_string(FPS) + " \n";

	buffer += "\n Filter Mode: " + to_string(world->GetRenderClass()->GetCellDisplayMode());

	switch (world->GetRenderClass()->GetCellDisplayMode())
	{
	case(Filter_NoFilter):
		buffer += "\n  The selected Cell is purpel";
		break;

	case(Filter_EnergyManager):
		buffer += "\n  Cyan correlates with Energy Managers\n  Yellow correlates with Oxygen Energy Managers\n";
		break;

	case(Filter_SplittingMembrane):
		buffer += "\n  Cyan correlates with Membranes\n  Yellow correlates with Splitting Managers\n";
		break;

	case(Filter_Flagellum):
		buffer += "\n  Cyan correlates with Max Speed\n  Yellow correlates with Acceleration Speed\n";
		break;

	case(Filter_CellHealth):
		buffer += "\n  Cyan correlates with Swelling\n  Yellow correlates with the Lack of ATP\n";
		break;

	}

	SetWindowText(textLabel, TEXT(buffer.c_str()));
}

LRESULT InfoWindow::Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

InfoWindow::~InfoWindow()
{
}
