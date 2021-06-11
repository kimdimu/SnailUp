// OpenGL_Templete.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "global.h"
#include "dsTexture.h"
#include <time.h>
#include "dsOpenALSoundManager.h"
#include "MyCharacter.h"
#include "MapObject.h"
//#include "BigStone.h"
#include "SmallStone_img.h"
#include "Endline.h"
#include "SnailAI.h"

#include <stdlib.h>

CFrameOpenGL  g_OpenGL;
HDC						g_hDC;

CMapObject    g_MapObj;
DWORD         g_tick;
NewSnail	s_snail;
NewSnail		s_snailblue; //ai blue
NewSnail		s_snailyellow; // ai yellow
NewSnail	s_snailred; // ai red
BigStone		s_bigstone; // ū��1
BigStone		s_bigstone2; // ū��2
SmallStone_img      s_smallstone; // ������1
SmallStone_img      s_smallstone2; // ������2
SmallStone_img      s_snailloseImg; // �� �̹���
SmallStone_img      s_snailwinImg; // �� �̹���
SmallStone_img      s_titleImg; // Ÿ��Ʋ �̹���
SmallStone_img      s_snailBwinImg; // b�� �̹���
SmallStone_img      s_snailYwinImg; // y�� �̹���
SmallStone_img      s_snailRwinImg; // r�� �̹���
SnailAI      s_snailBai; // 1p blue
SnailAI      s_snailYai; // 2p yellow
SnailAI      s_snailRai; // 3p red
Endline      s_endline; // ����� �̹���

void OnIdle();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS   wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "OpenGL_Templete";

	if (RegisterClass(&wndclass) == 0)
	{
		return 0;
	}

	RECT WindowRect;                            // �簢���� �»�ܰ� / ���ϴ� ���� ������
	WindowRect.left = (long)0;                    // ���� ���� 0���� ����
	WindowRect.right = (long)WINDOW_SIZE_WIDTH;               // ������ ���� ��û�� �ʺ�� ����
	WindowRect.top = (long)0;                     // ���� ���� 0���� ����
	WindowRect.bottom = (long)WINDOW_SIZE_HEIGHT;             // �Ʒ��� ���� ��û�� ���̷� ����

	DWORD        dwExStyle;                        // ������ Ȯ�� ��Ÿ��
	DWORD        dwStyle;                          // ������ ��Ÿ��

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // ������ Ȯ�� ��Ÿ��
	dwStyle = WS_OVERLAPPEDWINDOW;                    // �������� ��Ÿ��

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);        // ���� ��û�� ũ��� ������ ũ�⸦ �����Ѵ�

	HWND hwnd = CreateWindowEx(dwExStyle,                // �� ������� Ȯ�彺Ÿ��
		"OpenGL_Templete",							    // Ŭ���� �̸�
		"������ �����ϱ�",									   // ������ Ÿ��Ʋ
		WS_CLIPSIBLINGS |							    // �ʼ� ������ ��Ÿ��
		WS_CLIPCHILDREN |						        // �ʼ� ������ ��Ÿ��
		dwStyle,								    // ���õ� ������ ��Ÿ��
		0, 0,									       // â ��ġ
		WindowRect.right - WindowRect.left,			  // ������ â �ʺ� �����
		WindowRect.bottom - WindowRect.top,			   // ������ â ���̸� �����
		NULL,												     // �θ� ������ ����
		NULL,									     // �޴� ����
		hInstance,										// �ν��Ͻ�
		NULL);										 // WM_CREATE�� �ƹ��͵� �������� ����


	if (hwnd == NULL)
	{
		return 0;
	}
	

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	RECT rc;
	GetClientRect(hwnd, &rc);

	srand(time(NULL));

	//
	s_snail.Create("Character");
	s_snail.SetLayer(10);

	s_snailblue.Create("Character");
	s_snailblue.SetLayer(10);

	s_snailyellow.Create("Character"); 
	s_snailyellow.SetLayer(10);

	s_snailred.Create("Character");
	s_snailred.SetLayer(10);
	//
	s_snailBai.Create("CharacterB");
	s_snailBai.SetLayer(10);

	s_snailYai.Create("CharacterY");
	s_snailYai.SetLayer(10);

	s_snailRai.Create("CharacterR");
	s_snailRai.SetLayer(10);
	//
	s_bigstone.Create("S_StoneB");
	s_bigstone.SetLayer(10);

	s_bigstone2.Create("S_StoneB");
	s_bigstone2.SetLayer(10);
	//
	s_smallstone.Create("S_StoneS", "S_StoneB");
	s_smallstone.SetLayer(10);

	s_smallstone2.Create("S_StoneS", "S_StoneB");
	s_smallstone2.SetLayer(10);
	//
	s_snailloseImg.Create("S_SnailLose", "S_StoneB");
	s_snailloseImg.SetPosition(0, -900);
	s_snailloseImg.SetLayer(10);

	s_snailwinImg.Create("S_SnailWin", "S_StoneB");
	s_snailwinImg.SetPosition(0, -900);
	s_snailwinImg.SetLayer(10);

	s_titleImg.Create("Title", "S_StoneB");
	s_titleImg.SetPosition(0, -900);
	s_titleImg.SetLayer(10);

	s_snailBwinImg.Create("S_BlueWin", "S_StoneB");
	s_snailBwinImg.SetPosition(0, -900);
	s_snailBwinImg.SetLayer(10);

	s_snailYwinImg.Create("S_YellowWin", "S_StoneB");
	s_snailYwinImg.SetPosition(0, -900);
	s_snailYwinImg.SetLayer(10);

	s_snailRwinImg.Create("S_RedWin", "S_StoneB");
	s_snailRwinImg.SetPosition(0, -900);
	s_snailRwinImg.SetLayer(10);
	//
	s_endline.Create("S_Endline");
	s_endline.SetPosition(250 + rand() % 150, -900);
	s_endline.SetLayer(10);

	g_MapObj.Create("mapdata.txt");

	dsOpenALSoundManager* pSoundManger = GetOpenALSoundManager();
	dsSound* pSound = pSoundManger->LoadSound("./Resources/Sounds/William Tell Overture Finale.wav", true);
	if (pSound)
	{
		pSound->Play();
	}

	g_tick = GetTickCount();

	MSG msg;
	while (1)
	{
		//������ �޼����� ������� �޼����� ó���Ѵ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //�޼����� ���� ��� ���� ������ �����Ѵ�.
		{
			OnIdle();
		}
	}

	ReleaseDC(hwnd, g_hDC);
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//DWORD	tickCount = GetTickCount();

	switch (msg)
	{
	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			exit(0x1B);
			break;
		}
	}
	break;
	case WM_CREATE:
	{
		g_hDC = GetDC(hWnd);

		g_OpenGL.Create(g_hDC);
	}
	break;
	case WM_SIZE:
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);

		g_OpenGL.SetSize(width, height);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


void OnIdle()
{
	DWORD	tickCount = GetTickCount();
	DWORD tick = tickCount - g_tick;
	
	g_OpenGL.BeginRender();	
	
	g_MapObj.OnUpdate(tick);

	g_OpenGL.EndRender(g_hDC);

	g_tick = tickCount;
}


