#include "Input.h"
#include "Model.h"
#include "Camera.h"
#include "RenderClass.h"
#include "Include.h"

#include "CellInfoWindow.h"
#include "World.h"

Input::Input(RenderClass* RndCls, RECT rect)
{
	render = RndCls;
	wndRect = rect;
}

void Input::Key(bool down, WPARAM mes)
{
	wchar_t test = static_cast<wchar_t>(mes);

	switch (static_cast<wchar_t>(mes))
	{
	case('W'):
		keyW = down;
		break;

	case('A'):
		keyA = down;
		break;

	case('S'):
		keyS = down;
		break;

	case('D'):
		keyD = down;
		break;

	case('C'):
		keyC = down;
		break;

	case('V'):
		keyV = down;
		break;

	case('P'):
		if (down)
		{
			D3D11_RASTERIZER_DESC rasDesc;
			render->GetRasDesc(&rasDesc);

			if (rasDesc.FillMode == D3D11_FILL_SOLID)
				render->SetFillMode(D3D11_FILL_WIREFRAME);
			else
				render->SetFillMode(D3D11_FILL_SOLID)                      ;
		}
		break;

	case('G'):
		if (down)
		{
			cellInfoWindow->SetClosestCellAsCurrentCell();
		}
		break;

	case('F'):
		if (down)
		{
			cellInfoWindow->SwitchFollowingCurrentCell();
		}
		break;
	case('T'):
		if (down)
		{
			cellInfoWindow->SplitCurrentCell();
		}
		break;
	case('N'):
		if (down)
		{
			render->GetWorld()->AddCell(new Cell(render, render->GetWorld(), nullptr, rand() % (render->GetWorld()->GetSizeX() * render->GetWorld()->GetChunkSize()), rand() % (render->GetWorld()->GetSizeY() * render->GetWorld()->GetChunkSize()), rand() % (render->GetWorld()->GetSizeZ() * render->GetWorld()->GetChunkSize())));
		}
		break;
	}
}

void Input::Mouse(WPARAM wParam, int x, int y, HWND hWnd)
{
	if (!(GetKeyState(VK_MENU) & 0x8000))
	{
		int dx = x - (wndRect.right / 2);
		int dy = y - (wndRect.bottom / 2);

		render->GetCamera()->AddRotation(dy / mouseSense, dx / mouseSense, 0.0f);

		POINT centerPos;
		centerPos.x = (wndRect.right / 2);
		centerPos.y = (wndRect.bottom / 2);

		MapWindowPoints(hWnd, NULL, &centerPos, 1);
		SetCursorPos(centerPos.x, centerPos.y);
	}
}



void Input::RunTick(int t)
{
	x = 0;
	y = 0;
	z = 0;

	//determin how much we go in what direction regarding the camera rotation
	float Pitch = render->GetCamera()->GetPitch();

	TurnRadiantsIntoXandZComponent(Pitch, &rX, &rZ);

	//adding movement from the keys together
	if (keyW)
	{
		z += speed * t * rZ;
		x += speed * t * rX;
	}

	if (keyA)
	{
		z += speed * t * rX;
		x -= speed * t * rZ;
	}

	if (keyS)
	{
		z -= speed * t * rZ;
		x -= speed * t * rX;
	}

	if (keyD)
	{
		z -= speed * t * rX;
		x += speed * t * rZ;
	}

	if (keyC)
	{
		y += speed * t;
	}

	if (keyV)
	{
		y -= speed * t;
	}

	//if shift is pressed, we want to go faster
	if (HIWORD(GetKeyState(VK_SHIFT)))
	{
		x *= 2;
		y *= 2;
		z *= 2;
	}

	//keeping the Character move at a constant speed
	//the character will slow down however if, for example, A, D and W are pressed at the same time but i honestly dont care, that would be their own fault
	if ((keyW || keyS) && (keyA || keyD))
		render->GetCamera()->AddPosition(x / 2, y, z / 2, true);
	else
		render->GetCamera()->AddPosition(x, y, z, true);
}
