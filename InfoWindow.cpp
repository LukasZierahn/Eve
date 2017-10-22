#include "InfoWindow.h"
#include "include.h"
#include "world.h"

InfoWindow::InfoWindow(HWND hW, HINSTANCE hIns, World* w)
{
	hWnd = hW;
	hInstance = hIns;
	world = w;

	RECT rect;
	GetWindowRect(hWnd, &rect);
	textLabel = CreateWindow("STATIC", "not initilized", WS_VISIBLE | WS_CHILD | SS_LEFT, 0, 0, rect.right, 600, hWnd, NULL, hInstance, NULL);
	WriteInfoData();
}

int InfoWindow::WriteString(HDC hDC, string toWrite, int pos)
{

	TextOut(hDC, 10, pos * 20, toWrite.c_str(), toWrite.length());
	return ++pos;
}

void InfoWindow::WriteInfoData()
{
	string buffer = "";

	buffer = world->GetInfoWindowString();

	SetWindowText(textLabel, TEXT(buffer.c_str()));
}

InfoWindow::~InfoWindow()
{
}
